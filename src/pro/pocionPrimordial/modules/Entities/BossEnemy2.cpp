#include "BossEnemy2.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

BossEnemy2::BossEnemy2(const sf::Vector2f &position, TileMap *tileMap)
    : BossEnemy(position), tileMap(tileMap)
{
    if (!texture.loadFromFile("resources/sprites1.png"))
    {
        std::cerr << "Error cargando sprites1.png" << std::endl;
    }
    if (!bulletTexture.loadFromFile("resources/fuego-azul.png"))
    {
        std::cerr << "Error cargando fuego-azul.png" << std::endl;
    }
    if (!attackBuffer.loadFromFile("resources/ataque-boss-agua.ogg"))
    {
        std::cerr << "No se pudo cargar el sonido de ataque del boss.\n";
    }
    attackSound.setBuffer(attackBuffer);
    attackSound.setVolume(60.f);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(7, 454, 125, 220)); // Frame A (por defecto)
    sprite.setPosition(position);
    sprite.setScale(1.2f, 1.2f);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    previousPosition = position;
}

void BossEnemy2::update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight)
{
    storePreviousPosition();

    // Movimiento hacia el jugador
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0)
    {
        direction /= length;
        sf::Vector2f newPos = sprite.getPosition() + direction * speed * deltaTime;
        newPos.x = std::max(0.f, std::min(newPos.x, mapWidth - sprite.getGlobalBounds().width));
        newPos.y = std::max(0.f, std::min(newPos.y, mapHeight - sprite.getGlobalBounds().height));
        sprite.setPosition(newPos);
    }

    // Alternar entre dos frames cada 0.2 segundos
    if (animationClock.getElapsedTime().asSeconds() > 0.2f)
    {
        toggleFrame = !toggleFrame;
        animationClock.restart();

        if (toggleFrame)
            sprite.setTextureRect(sf::IntRect(7, 454, 125, 220)); // Frame A
        else
            sprite.setTextureRect(sf::IntRect(135, 454, 125, 220)); // Frame B
    }

    // Teletransportación
    teleportTimer += deltaTime;
    if (teleportTimer >= teleportInterval && tileMap)
    {
        sprite.setPosition(tileMap->getRandomWalkablePosition());
        teleportTimer = 0.f;
        spiralAttack = !spiralAttack;
    }

    // Actualizar balas
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        float angleRad = bulletAngles[i] * 3.14159f / 180.f;
        bullets[i].move(std::cos(angleRad) * bulletSpeeds[i] * deltaTime, std::sin(angleRad) * bulletSpeeds[i] * deltaTime);
        bulletSpeeds[i] = 200.f + 100.f * std::sin(attackTimer.getElapsedTime().asSeconds() * 2.f);
        sf::Vector2f pos = bullets[i].getPosition();
        if (pos.x < 0 || pos.x > mapWidth || pos.y < 0 || pos.y > mapHeight)
        {
            bullets.erase(bullets.begin() + i);
            bulletSpeeds.erase(bulletSpeeds.begin() + i);
            bulletAngles.erase(bulletAngles.begin() + i);
            --i;
        }
    }

    // Disparar
    if (attackTimer.getElapsedTime().asSeconds() > attackInterval)
    {
        attack(playerPosition);
    }

    if (spiralAttack)
    {
        spiralAngle += 360.f * deltaTime;
        if (spiralAngle >= 360.f)
            spiralAngle -= 360.f;
    }
}

void BossEnemy2::attack(sf::Vector2f playerPosition)
{
    bullets.clear();
    bulletSpeeds.clear();
    bulletAngles.clear();

    if (spiralAttack)
    {
        const int numBullets = 12;
        const float angleStep = 360.f / numBullets;
        for (int i = 0; i < numBullets; ++i)
        {
            sf::Sprite bullet;
            bullet.setTexture(bulletTexture);
            bullet.setPosition(sprite.getPosition());
            sf::FloatRect bounds = bullet.getLocalBounds();
            bullet.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            bullet.setScale(1.5f, 1.5f); // Escala más grande
            float angle = spiralAngle + i * angleStep;
            bullets.push_back(bullet);
            bulletSpeeds.push_back(200.f);
            bulletAngles.push_back(angle);
        }
    }
    else
    {
        const int numBullets = 8;
        const float angleStep = 360.f / numBullets;
        for (int i = 0; i < numBullets; ++i)
        {
            sf::Sprite bullet;
            bullet.setTexture(bulletTexture);
            bullet.setPosition(sprite.getPosition());
            sf::FloatRect bounds = bullet.getLocalBounds();
            bullet.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            bullet.setScale(1.5f, 1.5f); // Escala más grande
            float angle = i * angleStep;
            bullets.push_back(bullet);
            bulletSpeeds.push_back(200.f);
            bulletAngles.push_back(angle);
        }
    }

    attackTimer.restart();
    attackSound.play();
}

void BossEnemy2::render(sf::RenderWindow &window)
{
    window.draw(sprite);
    for (const auto &bullet : bullets)
    {
        window.draw(bullet);
    }
}

void BossEnemy2::drawInterpolated(sf::RenderWindow &window, float alpha)
{
    sf::Vector2f interpolatedPos = previousPosition + (sprite.getPosition() - previousPosition) * alpha;
    sf::Sprite interpolatedSprite = sprite;
    interpolatedSprite.setPosition(interpolatedPos);
    window.draw(interpolatedSprite);
    for (const auto &bullet : bullets)
    {
        window.draw(bullet);
    }
}

void BossEnemy2::storePreviousPosition()
{
    previousPosition = sprite.getPosition();
}

const std::vector<sf::Sprite> &BossEnemy2::getBullets() const
{
    return bullets;
}

void BossEnemy2::takeDamage(float damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

float BossEnemy2::getHealth() const
{
    return health;
}