#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Enemy
{
public:
    Enemy(sf::Vector2f position);
    virtual ~Enemy() = default;

    virtual void update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void attack(sf::Vector2f playerPosition) = 0;

    virtual void drawInterpolated(sf::RenderWindow &window, float alpha) = 0;
    virtual void storePreviousPosition() = 0;

    sf::FloatRect getBounds() const;

protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float attackCooldown;
    sf::Clock attackTimer;
};
