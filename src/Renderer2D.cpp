#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

static float vertices[] = {
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

Renderer2D::Renderer2D(int screenWidth, int screenHeight)
{
    shaderProgram = LoadShader("shaders/basic.vert", "shaders/basic.frag");

    projection = glm::ortho(
        0.0f, (float)screenWidth,
        0.0f, (float)screenHeight
    );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Renderer2D::~Renderer2D()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Renderer2D::DrawSprite(unsigned int textureID, float x, float y, float width, float height, glm::vec3 color)
{
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "projection"),
        1, GL_FALSE, glm::value_ptr(projection)
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "model"),
        1, GL_FALSE, glm::value_ptr(model)
    );

    glUniform3fv(
        glGetUniformLocation(shaderProgram, "color"),
        1, glm::value_ptr(color)
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniform3fv(glGetUniformLocation(shaderProgram, "spriteColor"), 1, glm::value_ptr(color));
    glUniform1i(glGetUniformLocation(shaderProgram, "image"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

unsigned int Renderer2D::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

unsigned int Renderer2D::LoadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Falha ao carregar sprite. " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}