#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entities/Entity.h"

class Obstacle : public Entity
{
private:
    float speed;
    unsigned int textureID;

public:
    Obstacle(glm::vec2 position, glm::vec2 size, float moveSpeed, unsigned int textureID);
    ~Obstacle() override = default;

    void Update(float deltaTime) override;
    void Render(Renderer2D& renderer) override;

    bool IsOffScreen() const; 
};

#endif