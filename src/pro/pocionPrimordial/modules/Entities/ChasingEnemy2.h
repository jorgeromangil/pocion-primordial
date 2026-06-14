#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include "../Map/TileMap.h"

class ChasingEnemy2 : public Enemy
{
public:
    ChasingEnemy2(sf::Vector2f position, float speed, TileMap *tileMap = nullptr);
    void update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight) override;
    void render(sf::RenderWindow &window) override;
    void attack(sf::Vector2f playerPosition) override { (void)playerPosition; } // No dispara

    void updateAnimation(float deltaTime);

    void drawInterpolated(sf::RenderWindow &window, float alpha) override;
    void storePreviousPosition() override;

private:
    float speed;
    float animationTimer = 0.f;
    float frameDuration = 0.6f;
    bool frameToggle = false;
    const int frameWidth = 60;
    const int frameHeight = 75;
    sf::Vector2f previousPosition;
    TileMap *tileMap; // Para verificar colisiones
};