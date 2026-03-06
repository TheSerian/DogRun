#ifndef GAME_H
#define GAME_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>

#include "entities/Entity.h"
#include "entities/Player.h"
#include "entities/Obstacle.h"
#include "Renderer2D.h"

enum class GameState
{
    IDLE,
    Playing,
    GameOver
};

class Game
{
public:
    Game(int width, int height);
    ~Game();

    void ProcessInput(GLFWwindow* window);
    void Update(float deltaTime);
    void Render();
    void Reset();
    void RenderScore();
    void RenderBestScore();

private:
    bool CheckCollision(Entity* one, Entity* two);


    int screenWidth;
    int screenHeight;

    GameState state;

    Renderer2D* renderer;
    Player* player;
    std::vector<Entity*> entities;

    unsigned int textureGroundBottom;
    unsigned int textureGroundTop;
    unsigned int textureObstacle;
    unsigned int textureDogDead;
    unsigned int textureGameOver;
    unsigned int textureRestart;
    unsigned int textureStart;
    unsigned int textureScoreLabel;
    unsigned int textureBestLabel;

    std::array<unsigned int, 10> digitTextures;

    float spawnTimer;
    float spawnInterval;
    float baseSpawnInterval = 2.0f;
    float score;
    float initialGameSpeed;
    float gameSpeed;
    float difficultyMultiplier = 1.0f;
    int bestScore;
};

#endif