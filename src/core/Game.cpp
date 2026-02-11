#include "core/Game.h"

Game::Game(int width, int height)
    : screenWidth(width),
      screenHeight(height),
      state(GameState::Playing)
{
    renderer = new Renderer2D(screenWidth, screenHeight);

    player = new Player({100.0f, 100.0f}, {50.0f, 50.0f}, *renderer);
    entities.push_back(player);
}

Game::~Game()
{
    for (Entity* e : entities)
        delete e;

    delete renderer;
}

void Game::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    player->ProcessInput(window);
}

void Game::Update(float deltaTime)
{
    for (Entity* e : entities)
        e->Update(deltaTime);
}

void Game::Render()
{
    for (Entity* e : entities)
        e->Render(*renderer);
}
