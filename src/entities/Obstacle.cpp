#include "entities/Obstacle.h"
#include "Renderer2D.h"

Obstacle::Obstacle(glm::vec2 position, glm::vec2 size, float moveSpeed, unsigned int textureID)
    : Entity(position, size), speed(moveSpeed), textureID(textureID)
{
}

void Obstacle::Update(float deltaTime)
{
    position.x -= speed * deltaTime;
}

void Obstacle::Render(Renderer2D& renderer)
{
    renderer.DrawSprite(textureID, position.x, position.y, size.x, size.y);
}

bool Obstacle::IsOffScreen() const
{
    return (position.x + size.x) < 0.0f; 
}