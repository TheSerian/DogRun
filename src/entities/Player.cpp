#include "entities/Player.h"

Player::Player(glm::vec2 pos, glm::vec2 size, Renderer2D& renderer)
    : Entity(pos, size),
      velocityY(0.0f),
      onGround(true),
      currentState(PlayerState::IDLE),
      animationTimer(0.0f),
      useRunFrame1(true)
{
    textureIdle = renderer.LoadTexture("textures/dog_idle.png");
    textureRun1 = renderer.LoadTexture("textures/dog_walk1.png");
    textureRun2 = renderer.LoadTexture("textures/dog_walk2.png");
    textureHit  = renderer.LoadTexture("textures/dog_dead.png");
}

void Player::ProcessInput(GLFWwindow* window)
{
    if (currentState == PlayerState::GAME_OVER)
        return;

    if (currentState == PlayerState::IDLE){
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            currentState = PlayerState::RUNNING;
        }   
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
    {
        velocityY = 400.0f;
        onGround = false;
        currentState = PlayerState::RUNNING;
    }
}

void Player::Update(float deltaTime)
{
    if (currentState == PlayerState::GAME_OVER)
        return;

    velocityY -= 800.0f * deltaTime;
    position.y += velocityY * deltaTime;

    if (position.y <= 300.0f)
    {
        position.y = 300.0f;
        velocityY = 0.0f;
        onGround = true;
    }

    if (currentState == PlayerState::RUNNING){
        animationTimer += deltaTime;

        if (animationTimer >= 0.1f){
            animationTimer = 0.0f;
            useRunFrame1 = !useRunFrame1;
        }
    }
}

void Player::Render(Renderer2D& renderer)
{
    unsigned int currentTexture = textureIdle;

    switch (currentState)
    {
        case PlayerState::IDLE:
            currentTexture = textureIdle;
            break;

        case PlayerState::RUNNING:
            if (!onGround){
                currentTexture = textureRun2;
            } else {
                currentTexture = useRunFrame1 ? textureRun1 : textureRun2;
            }
            break;
        
        case PlayerState::GAME_OVER:
            currentTexture = textureHit;
            break;
    }

    renderer.DrawSprite(currentTexture, position.x, position.y, size.x, size.y);
}