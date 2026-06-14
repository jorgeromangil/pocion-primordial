#ifndef BOSSENEMY2_H
#define BOSSENEMY2_H

#include "BossEnemy.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Map/TileMap.h"
#include <SFML/Audio.hpp>

class BossEnemy2 : public BossEnemy
{
public:
    BossEnemy2(const sf::Vector2f &position, TileMap *tileMap = nullptr);
    void update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight) override;
    void render(sf::RenderWindow &window) override;
    void attack(sf::Vector2f playerPosition) override;
    void drawInterpolated(sf::RenderWindow &window, float alpha) override;
    void storePreviousPosition() override;

    const std::vector<sf::Sprite> &getBullets() const;
    void takeDamage(float damage);
    float getHealth() const;
    sf::Clock circleDamageClock;

private:
    sf::Texture bulletTexture; // Textura para las balas (fuego-azul.png)
    sf::Vector2f previousPosition;
    float health = 200.f;
    float speed = 50.f;
    std::vector<sf::Sprite> bullets;
    std::vector<float> bulletSpeeds;
    std::vector<float> bulletAngles; // Ángulos para movimiento de balas
    float attackInterval = 2.0f;
    bool spiralAttack = false;
    float spiralAngle = 0.f;
    float teleportTimer = 0.f;
    float teleportInterval = 5.0f;
    TileMap *tileMap;
    sf::Clock animationClock;
    bool toggleFrame = false;
    sf::SoundBuffer attackBuffer;
    sf::Sound attackSound;
};

#endif // BOSSENEMY2_H