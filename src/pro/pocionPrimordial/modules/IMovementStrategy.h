#ifndef IMOVEMENTSTRATEGY_H
#define IMOVEMENTSTRATEGY_H

#include <SFML/Graphics.hpp>
#include "Map/TileMap.h"

class IMovementStrategy
{
public:
    virtual void move(sf::Sprite &sprite, float deltaTime) = 0;
    virtual void setTileMap(TileMap *map) = 0;
    virtual void setSpeedMultiplier(float multiplier) = 0;
    virtual ~IMovementStrategy() = default;
};

#endif