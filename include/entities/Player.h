#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "entities/Entity.h"
#include "Renderer2D.h"

enum class PlayerState {
    IDLE,
    RUNNING,
    GAME_OVER
};

class Player : public Entity
{
public:
    Player(glm::vec2 pos, glm::vec2 size, Renderer2D& renderer);

    void ProcessInput(GLFWwindow* window);
    void Update(float deltaTime) override;
    void Render(Renderer2D& renderer) override;

    void SetState(PlayerState newState){currentState = newState;}
    PlayerState GetState() const {return currentState;}

private:
    float velocityY;
    float speedX;
    bool onGround;

    unsigned int textureIdle;
    unsigned int textureRun1;
    unsigned int textureRun2;
    unsigned int textureHit;

    PlayerState currentState;
    float animationTimer;
    bool useRunFrame1;
};

#endif
