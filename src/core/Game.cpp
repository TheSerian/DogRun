#include "core/Game.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <glm/glm.hpp>

Game::Game(int width, int height)
    : screenWidth(width),
      screenHeight(height),
      state(GameState::IDLE),
      spawnTimer(0.0f),
      spawnInterval(2.0f),
      score(0.0f),
      bestScore(0),
      initialGameSpeed(300.0f),
      gameSpeed(initialGameSpeed)
{
    renderer = new Renderer2D(screenWidth, screenHeight);
    srand(static_cast<unsigned int>(time(nullptr)));

    textureGroundBottom = renderer->LoadTexture("textures/ground_bottom.png");
    textureGroundTop    = renderer->LoadTexture("textures/ground_top.png");
    textureObstacle     = renderer->LoadTexture("textures/obstacle.png");
    textureDogDead      = renderer->LoadTexture("textures/dog_dead.png");
    textureGameOver     = renderer->LoadTexture("textures/game_over.png");
    textureRestart      = renderer->LoadTexture("textures/restart.png");
    textureStart        = renderer->LoadTexture("textures/start.png");
    textureScoreLabel   = renderer->LoadTexture("textures/ui/score.png");
    textureBestLabel    = renderer->LoadTexture("textures/ui/best.png");

    for (int i = 0; i < 10; ++i) {
        std::string path = "textures/ui/" + std::to_string(i) + ".png";
        digitTextures[i] = renderer->LoadTexture(path.c_str());
    }

    player = new Player({20.0f, 300.0f}, {50.0f, 50.0f}, *renderer);
    entities.push_back(player);
}

Game::~Game()
{
    for (Entity* e : entities) {
        delete e;
    }

    delete renderer;
}

void Game::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static bool restartPressed = false;

    if (state == GameState::IDLE) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            state = GameState::Playing;
            player->SetState(PlayerState::RUNNING);
        }
        return;
    }

    if (state == GameState::Playing) {
        player->ProcessInput(window);
        return;
    }

    if (state == GameState::GameOver) {
        bool enterPressed =
            glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS;

        if (enterPressed) {
            if (!restartPressed) {
                Reset();
                restartPressed = true;
            }
        } else {
            restartPressed = false;
        }
    }
}

void Game::Update(float deltaTime)
{
    if (state != GameState::Playing) {
        return;
    }

    score += deltaTime * 10.0f;

    difficultyMultiplier = 1.0f + (score / 400.0f);
    gameSpeed = initialGameSpeed * difficultyMultiplier;
    spawnInterval = baseSpawnInterval / difficultyMultiplier;

    spawnTimer += deltaTime;

    if (static_cast<int>(score) > bestScore) {
        bestScore = static_cast<int>(score);
    }

    if (spawnTimer >= spawnInterval) {
        Obstacle* newObstacle = new Obstacle(
            {800.0f, 300.0f},
            {50.0f, 50.0f},
            gameSpeed,
            textureObstacle
        );

        entities.push_back(newObstacle);
        spawnTimer = 0.0f;
    }

    for (Entity* e : entities) {
        e->Update(deltaTime);
    }

    for (auto it = entities.begin(); it != entities.end(); ) {
        Obstacle* obs = dynamic_cast<Obstacle*>(*it);

        if (obs != nullptr) {
            if (CheckCollision(player, obs)) {
                state = GameState::GameOver;
                player->SetState(PlayerState::GAME_OVER);
                break;
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

    for (float y = 0.0f; y < groundLevel; y += tileSize) {
        for (float x = 0.0f; x < static_cast<float>(screenWidth); x += tileSize) {
            if (y >= groundLevel - tileSize) {
                renderer->DrawSprite(textureGroundTop, x, y, tileSize, tileSize);
            } else {
                renderer->DrawSprite(textureGroundBottom, x, y, tileSize, tileSize);
            }
        }
    }

    for (Entity* e : entities) {
        if (state == GameState::GameOver && e == player) {
            renderer->DrawSprite(
                textureDogDead,
                player->GetPosition().x,
                player->GetPosition().y,
                player->GetSize().x,
                player->GetSize().y
            );
        } else {
            e->Render(*renderer);
        }
    }

    if (state == GameState::IDLE) {
        float width = 500.0f;
        float height = 250.0f;

        renderer->DrawSprite(
            textureStart,
            screenWidth / 2.0f - width / 2.0f,
            screenHeight / 2.0f - height / 2.0f + 100.0f,
            width,
            height
        );
    }

    if (state == GameState::GameOver) {
        float screenCenterX = screenWidth / 2.0f;
        float screenCenterY = screenHeight / 2.0f;

        float goWidth = 400.0f;
        float goHeight = 200.0f;
        float goOffsetY = 100.0f;

        renderer->DrawSprite(
            textureGameOver,
            screenCenterX - goWidth / 2.0f,
            screenCenterY - goHeight / 2.0f + goOffsetY,
            goWidth,
            goHeight
        );

        float restartWidth = 70.0f;
        float restartHeight = 70.0f;
        float restartOffsetY = goOffsetY - 30.0f;

        renderer->DrawSprite(
            textureRestart,
            screenCenterX - restartWidth / 2.0f,
            screenCenterY - goHeight / 2.0f + restartOffsetY,
            restartWidth,
            restartHeight
        );
    }

    RenderBestScore();
    RenderScore();
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

    for (Entity* e : entities) {
        delete e;
    }
    entities.clear();

    player = new Player({20.0f, 300.0f}, {50.0f, 50.0f}, *renderer);
    player->SetState(PlayerState::RUNNING);
    entities.push_back(player);

    score = 0.0f;
    spawnTimer = 0.0f;
    spawnInterval = baseSpawnInterval;
    difficultyMultiplier = 1.0f;
    gameSpeed = initialGameSpeed;
}

void Game::RenderScore()
{
    int scoreInt = static_cast<int>(score);
    std::string scoreText = std::to_string(scoreInt);

    float y = 520.0f;
    float digitWidth = 24.0f;
    float digitHeight = 32.0f;
    float spacing = 4.0f;
    float marginRight = 20.0f;

    float labelWidth = 90.0f;
    float labelHeight = 28.0f;
    float labelSpacing = 10.0f;

    float numberWidth = scoreText.length() * digitWidth +
                        (scoreText.length() - 1) * spacing;

    float totalWidth = labelWidth + labelSpacing + numberWidth;
    float startX = screenWidth - marginRight - totalWidth;

    renderer->DrawSprite(
        textureScoreLabel,
        startX,
        y + 2.0f,
        labelWidth,
        labelHeight
    );

    startX += labelWidth + labelSpacing;

    for (char c : scoreText) {
        int digit = c - '0';

        if (digit < 0 || digit > 9) {
            continue;
        }

        if (digitTextures[digit] != 0) {
            renderer->DrawSprite(
                digitTextures[digit],
                startX,
                y,
                digitWidth,
                digitHeight
            );
        }

        startX += digitWidth + spacing;
    }
}

void Game::RenderBestScore()
{
    std::string bestText = std::to_string(bestScore);

    float y = 560.0f;
    float digitWidth = 20.0f;
    float digitHeight = 28.0f;
    float spacing = 4.0f;
    float marginRight = 20.0f;

    float labelWidth = 70.0f;
    float labelHeight = 24.0f;
    float labelSpacing = 10.0f;

    float numberWidth = bestText.length() * digitWidth +
                        (bestText.length() - 1) * spacing;

    float totalWidth = labelWidth + labelSpacing + numberWidth;
    float startX = screenWidth - marginRight - totalWidth;

    renderer->DrawSprite(
        textureBestLabel,
        startX,
        y + 2.0f,
        labelWidth,
        labelHeight
    );

    startX += labelWidth + labelSpacing;

    for (char c : bestText) {
        int digit = c - '0';

        if (digit < 0 || digit > 9) {
            continue;
        }

        if (digitTextures[digit] != 0) {
            renderer->DrawSprite(
                digitTextures[digit],
                startX,
                y,
                digitWidth,
                digitHeight
            );
        }

        startX += digitWidth + spacing;
    }
}