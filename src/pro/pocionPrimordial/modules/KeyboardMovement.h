#ifndef KEYBOARD_MOVEMENT_H
#define KEYBOARD_MOVEMENT_H

#include "IMovementStrategy.h"
#include "Map/TileMap.h"

class KeyboardMovement : public IMovementStrategy
{
public:
    KeyboardMovement();
    void move(sf::Sprite &sprite, float deltaTime) override;
    void setTileMap(TileMap *map) override;
    void setSpeedMultiplier(float multiplier) override;

private:
    TileMap *tileMap = nullptr;
    float speedMultiplier = 1.f;
};

#endif