#pragma once
#include "Enemy.h"
#include <vector>

class ShootingEnemy : public Enemy
{
public:
    ShootingEnemy(sf::Vector2f position);
    void update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight) override;    
    void attack(sf::Vector2f playerPosition) override;
    const std::vector<sf::CircleShape> &getBullets() const;
    void drawInterpolated(sf::RenderWindow &window, float alpha) override;
    void storePreviousPosition() override;

    void render(sf::RenderWindow &window) override;
private:
    std::vector<sf::CircleShape> bullets;
    std::vector<sf::Vector2f> bulletDirections;
    std::vector<sf::Vector2f> previousBulletPositions;

    float bulletSpeed;
    float bulletDelay;

    sf::Vector2f previousPosition;
};
