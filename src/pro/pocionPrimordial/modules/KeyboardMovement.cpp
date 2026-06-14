#include "KeyboardMovement.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

KeyboardMovement::KeyboardMovement()
    : tileMap(nullptr), speedMultiplier(1.f)
{
}

void KeyboardMovement::setTileMap(TileMap *map)
{
    tileMap = map;
}

void KeyboardMovement::move(sf::Sprite &sprite, float deltaTime)
{
    if (!tileMap)
        return;

    float speed = 200.f * speedMultiplier * deltaTime;
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed;

    if (movement == sf::Vector2f(0.f, 0.f))
        return;

    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += movement.x;
    bounds.top += movement.y;

    int tileW = tileMap->getTileWidth();
    int tileH = tileMap->getTileHeight();

    const int samples = 6;

    for (int i = 0; i < samples; ++i)
    {
        float t = i / static_cast<float>(samples - 1);

        float x = bounds.left + t * bounds.width;
        if (!tileMap->isWalkable(x / tileW, bounds.top / tileH) ||
            !tileMap->isWalkable(x / tileW, (bounds.top + bounds.height) / tileH))
        {
            return;
        }

        float y = bounds.top + t * bounds.height;
        if (!tileMap->isWalkable(bounds.left / tileW, y / tileH) ||
            !tileMap->isWalkable((bounds.left + bounds.width) / tileW, y / tileH))
        {
            return;
        }
    }

    sprite.move(movement);
}

void KeyboardMovement::setSpeedMultiplier(float multiplier)
{
    speedMultiplier = multiplier;
}