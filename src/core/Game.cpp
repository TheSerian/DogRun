#include "core/Game.h"

#include <cstdlib>
#include <ctime>

Game::Game(int width, int height)
    : screenWidth(width),
      screenHeight(height),
      state(GameState::Playing),
      spawnTimer(0.0f),
      spawnInterval(2.0f),
      score(0.0f),
      initialGameSpeed(300.0f),
      gameSpeed(initialGameSpeed)
{
    renderer = new Renderer2D(screenWidth, screenHeight);
    srand(static_cast<unsigned int>(time(nullptr)));

    textureGroundBottom = renderer->LoadTexture("textures/ground_bottom.png");
    textureGroundTop = renderer->LoadTexture("textures/ground_top.png");
    textureObstacle = renderer->LoadTexture("textures/obstacle.png");
    textureDogDead = renderer->LoadTexture("textures/dog_dead.png");
    textureGameOver = renderer->LoadTexture("textures/game_over.png");
    textureRestart  = renderer->LoadTexture("textures/restart.png");

    player = new Player({20.0f, 300.0f}, {50.0f, 50.0f}, *renderer);
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

    static bool enterPressed = false;

    if (state == GameState::GameOver && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (!enterPressed)
        {
            Reset();
            enterPressed = true;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
    {
        enterPressed = false;
    }
}

void Game::Update(float deltaTime)
{
    if (state != GameState::Playing)
        return;

    spawnTimer += deltaTime;

    if (spawnTimer >= spawnInterval)
    {
        Obstacle* newObstacle = new Obstacle(
            {800.0f, 300.0f},
            {50.0f, 50.0f},
            200.0f,
            textureObstacle
        );

        entities.push_back(newObstacle);
        spawnTimer = 0.0f;

        float minTime = 1.0f;
        float maxTime = 3.0f;
        spawnInterval = minTime +
            static_cast<float>(rand()) /
            (static_cast<float>(RAND_MAX / (maxTime - minTime)));
    }

    for (Entity* e : entities)
        e->Update(deltaTime);

    for (auto it = entities.begin(); it != entities.end(); )
    {
        Obstacle* obs = dynamic_cast<Obstacle*>(*it);

        if (obs != nullptr)
        {
            if (CheckCollision(player, obs))
            {
                state = GameState::GameOver;
            }

            if (obs->IsOffScreen())
            {
                delete obs;
                it = entities.erase(it);
                continue;
            }
        }

        ++it;
    }

    score += deltaTime * 10.0f;
}

void Game::Render()
{
    float tileSize = 50.0f;
    float groundLevel = 300.0f;

    for (float y = 0.0f; y < groundLevel; y += tileSize)
    {
        for (float x = 0.0f; x < (float)screenWidth; x += tileSize)
        {
            if (y >= groundLevel - tileSize)
                renderer->DrawSprite(textureGroundTop, x, y, tileSize, tileSize);
            else
                renderer->DrawSprite(textureGroundBottom, x, y, tileSize, tileSize);
        }
    }

    for (Entity* e : entities)
    {
        if (state == GameState::GameOver && e == player)
        {
            renderer->DrawSprite(
                textureDogDead,
                player->GetPosition().x,
                player->GetPosition().y,
                player->GetSize().x,
                player->GetSize().y
            );
        }
        else
        {
            e->Render(*renderer);
        }
    }

    if (state == GameState::GameOver)
    {
        float screenCenterX = screenWidth / 2.0f;
        float screenCenterY = screenHeight / 2.0f;

        float goWidth = 400.0f;
        float goHeight = 200.0f;
        float goOffsetY = 100.0f;
        renderer->DrawSprite(
            textureGameOver,
            screenCenterX - goWidth / 2,
            screenCenterY - goHeight / 2 + goOffsetY,
            goWidth,
            goHeight
        );

        float restartWidth = 70.0f;  
        float restartHeight = 70.0f;  
        float restartOffsetY = goOffsetY - 30.0f; 
        renderer->DrawSprite(
            textureRestart,
            screenCenterX - restartWidth / 2,
            screenCenterY - goHeight / 2 + restartOffsetY,
            restartWidth,
            restartHeight
        );
    }
}

bool Game::CheckCollision(Entity* one, Entity* two)
{
    glm::vec2 oneCenter = one->GetPosition() + one->GetSize() * 0.5f;
    glm::vec2 twoCenter = two->GetPosition() + two->GetSize() * 0.5f;

    glm::vec2 oneHalf = one->GetSize() * 0.5f;
    glm::vec2 twoHalf = two->GetSize() * 0.5f;

    float deltaX = std::abs(oneCenter.x - twoCenter.x);
    float deltaY = std::abs(oneCenter.y - twoCenter.y);

    bool collisionX = deltaX <= (oneHalf.x + twoHalf.x);
    bool collisionY = deltaY <= (oneHalf.y + twoHalf.y);

    return collisionX && collisionY;
}

void Game::Reset()
{
    state = GameState::Playing;

    for (Entity* e : entities)
        delete e;

    entities.clear();

    player = new Player({20.0f, 290.0f}, {50.0f, 50.0f}, *renderer);
    entities.push_back(player);

    score = 0.0f;
    spawnTimer = 0.0f;
    spawnInterval = 2.0f;
    gameSpeed = initialGameSpeed;
}