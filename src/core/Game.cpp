#include "core/Game.h"

#include <cstdlib>
#include <ctime>

Game::Game(int width, int height)
    : screenWidth(width),
      screenHeight(height),
      state(GameState::Playing)
{
    renderer = new Renderer2D(screenWidth, screenHeight);
    srand(static_cast<unsigned int>(time(nullptr)));

    textureGroundBottom = renderer->LoadTexture("textures/ground_bottom.png");
    textureGroundTop = renderer->LoadTexture("textures/ground_top.png");
    textureObstacle = renderer->LoadTexture("textures/obstacle.png");
    textureDogDead = renderer->LoadTexture("textures/dog_dead.png");

    player = new Player({100.0f, 400.0f}, {50.0f, 50.0f}, *renderer);
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
    if (state != GameState::Playing)
        return;

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        Obstacle* newObstacle = new Obstacle({800.0f, 300.0f}, {50.0f, 50.0f}, 200.f, textureObstacle);
        entities.push_back(newObstacle);
        spawnTimer = 0.0f;
        float minTime = 1.0f;
        float maxTime = 5.0f;
        spawnInterval = minTime + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxTime - minTime)));
    }

    for (Entity* e : entities)
        e->Update(deltaTime);
    
    for (auto it = entities.begin(); it != entities.end(); ) {
        Obstacle* obs = dynamic_cast<Obstacle*>(*it);

        if (obs != nullptr) {
            if (CheckCollision(player, obs)) {
                state = GameState::GameOver;
            }
            if (obs->IsOffScreen()) {
                delete obs;
                it = entities.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void Game::Render()
{

    float tileSize = 50.0f;
    float groundLevel = 300.0f;

    for (float y = 0.0f; y < 300.0f; y += tileSize)
    {
        for (float x = 0.0f; x < 800.0f; x += tileSize)
        {
            if (y >= groundLevel - tileSize){
                renderer->DrawSprite(textureGroundTop, x, y, tileSize, tileSize);
            }
            else{
                renderer->DrawSprite(textureGroundBottom, x, y, tileSize, tileSize);
            }
        }
    }

    for (Entity* e : entities) {
        if (state == GameState::GameOver && e == player) {
            renderer->DrawSprite(textureDogDead,
            player->GetPosition().x,
            player->GetPosition().y,
            player->GetSize().x,
            player->GetSize().y);
        } else {
            e->Render(*renderer);
        }
    }
}

bool Game::CheckCollision(Entity* one, Entity* two)
{
    bool collisionX = one->GetPosition().x + one->GetSize().x >= two->GetPosition().x &&
                      two->GetPosition().x + two->GetSize().x >= one->GetPosition().x;

    bool collisionY = one->GetPosition().y + one->GetSize().y >= two->GetPosition().y &&
                      two->GetPosition().y + two->GetSize().y >= one->GetPosition().y;

    return collisionX && collisionY;
}