#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "IMovementStrategy.h"
#include "../Map/TileMap.h"
#include <SFML/Audio.hpp>


class Character
{

public:
    Character();
    void setMovementStrategy(IMovementStrategy *strategy);
    void move(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderWindow &window);
    sf::Sprite getSprite() const;
    void shoot();
    void removeBullet(size_t index);
    void updateBullets(float deltaTime, const TileMap &tileMap);
    std::vector<sf::ConvexShape> &getMutableBullets();
    void renderBullets(sf::RenderWindow &window);
    void drawInterpolated(sf::RenderWindow &window, float alpha);

    float getHealth() const { return health; }
    void takeDamage(float damage);
    void heal(float amount);

    void applySpeedBoost(float multiplier, float duration);
    void applyFireRateBoost(float multiplier, float duration);
    void applyHealthRegen(float value, float duration); // Nuevo
    void updateEffects(float deltaTime);

    bool isSpeedBoostActive() const { return speedBoostDuration > 0.f && speedBoostTimer.getElapsedTime().asSeconds() <= speedBoostDuration; }
    bool isFireRateBoostActive() const { return fireRateBoostDuration > 0.f && fireRateBoostTimer.getElapsedTime().asSeconds() <= fireRateBoostDuration; }
    bool isHealthRegenActive() const { return healthRegenDuration > 0.f && healthRegenTimer.getElapsedTime().asSeconds() <= healthRegenDuration; } // Nuevo
    float getSpeedBoostTimeRemaining() const { return isSpeedBoostActive() ? speedBoostDuration - speedBoostTimer.getElapsedTime().asSeconds() : 0.f; }
    float getFireRateBoostTimeRemaining() const { return isFireRateBoostActive() ? fireRateBoostDuration - fireRateBoostTimer.getElapsedTime().asSeconds() : 0.f; }
    float getHealthRegenTimeRemaining() const { return isHealthRegenActive() ? healthRegenDuration - healthRegenTimer.getElapsedTime().asSeconds() : 0.f; } // Nuevo


    //Arma circular
    void initializeCircularWeapon();
    void updateCircularWeapon();
    enum class WeaponType { Bullets, Circular };
    WeaponType getCurrentWeapon() const { return currentWeapon; }
    void switchWeapon();
    const sf::CircleShape& getCircularWeapon() const;
   

private:
    sf::Sprite sprite;
    sf::Texture texture;
    IMovementStrategy *movementStrategy;
    std::vector<sf::ConvexShape> bullets;
    std::vector<sf::Vector2f> bulletDirections;
    float bulletSpeed = 350.0f;
    sf::Clock shootTimer;
    float shootCooldown = 2.5f;
    float baseShootCooldown = 2.5f;
    int frame = 0;
    float animationTimer = 0.f;
    float frameDuration = 0.15f;
    sf::Vector2f previousPosition;
    sf::ConvexShape bulletTemplate;
    float health = 100.f;
    float maxHealth = 100.f;
    sf::Clock speedBoostTimer;
    sf::Clock fireRateBoostTimer;
    sf::Clock healthRegenTimer; // Nuevo
    float speedBoostDuration = 0.f;
    float fireRateBoostDuration = 0.f;
    float healthRegenDuration = 0.f; // Nuevo
    float healthRegenValue = 0.f;    // Nuevo
    sf::SoundBuffer attackSoundBuffer;
    sf::Sound attackSound;

    void initializeBulletShape();

    enum Direction
    {
        Down = 0,
        Left = 1,
        Up = 2,
        Right = 3
    };
    Direction currentDirection = Down;
       // Arma circular
    sf::CircleShape circularWeapon;
    float circularWeaponRadius = 110.f;
    float circularWeaponDamagePercent = 0.1f;
    WeaponType currentWeapon = WeaponType::Circular;
 
};

#endif