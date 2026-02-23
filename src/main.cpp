#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer2D.h"
#include "core/Game.h"

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "DogRun", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Renderer2D renderer(800, 600);
    Game game(800, 600);

    float lastFrame = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        game.ProcessInput(window);
        game.Update(deltaTime);

        std::string title = "DogRun - Score: " + std::to_string(game.GetScore());
        glfwSetWindowTitle(window, title.c_str());

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
