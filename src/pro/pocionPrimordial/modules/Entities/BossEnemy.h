#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Map/TileMap.h"
#include <SFML/Audio.hpp>

class BossEnemy : public Enemy
{
public:
    BossEnemy(const sf::Vector2f &position, TileMap *tileMap = nullptr);
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
    sf::Texture bulletTexture; // Textura para las balas (fuego-naranja.png)
    sf::Vector2f previousPosition;
    float health = 200.f;
    float speed = 50.f;
    std::vector<sf::Sprite> bullets;
    std::vector<float> bulletSpeeds;
    std::vector<float> bulletAngles; // Ángulos para movimiento de balas
    std::vector<float> bulletTimers; // Tiempo de vida de proyectiles parados
    std::vector<bool> bulletStates;  // true: moviéndose, false: parado
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

#endif // BOSSENEMY_H