#ifndef GAME_H
#define GAME_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

private:
    GameState state;

    int screenWidth;
    int screenHeight;

    Renderer2D* renderer;

    std::vector<Entity*> entities;
    Player* player;

    unsigned int textureDogDead;
    unsigned int textureGroundBottom;
    unsigned int textureGroundTop;
    unsigned int textureObstacle;
    unsigned int textureGameOver;
    unsigned int textureRestart;

    std::vector<Obstacle> obstacles;
    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;

    float score;

    float gameSpeed;
    float initialGameSpeed;

    bool CheckCollision(Entity* one, Entity* two);
};

#endif