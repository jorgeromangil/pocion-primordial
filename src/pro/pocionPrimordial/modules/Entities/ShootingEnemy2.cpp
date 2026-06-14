#include "ShootingEnemy2.h"
#include <iostream>
#include <cmath>

ShootingEnemy2::ShootingEnemy2(sf::Vector2f position)
    : Enemy(position), bulletSpeed(250.0f), bulletDelay(0.5f)
{
    attackCooldown = 1.0f;

    if (!texture.loadFromFile("resources/sprites2.png"))
    {
        std::cerr << "Error cargando la textura del ShootingEnemy2" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(4, 4+87, 60, 75));
    sprite.setPosition(position);
    previousPosition = position;
}

void ShootingEnemy2::update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight)
{
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        previousBulletPositions[i] = bullets[i].getPosition();
        bullets[i].move(bulletDirections[i] * bulletSpeed * deltaTime);
    }

    // Verificar si las balas están fuera de los límites del mapa
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        if (bullets[i].getPosition().x < 0 || bullets[i].getPosition().x > mapWidth ||
            bullets[i].getPosition().y < 0 || bullets[i].getPosition().y > mapHeight)
        {
            bullets.erase(bullets.begin() + i);
            bulletDirections.erase(bulletDirections.begin() + i);
            previousBulletPositions.erase(previousBulletPositions.begin() + i);
            --i;
        }
    }

    if (attackTimer.getElapsedTime().asSeconds() > attackCooldown)
    {
        attack(playerPosition);
        attackTimer.restart();
    }
}

void ShootingEnemy2::render(sf::RenderWindow &window)
{
    window.draw(sprite);
    for (const auto &bullet : bullets)
    {
        window.draw(bullet);
    }
}

void ShootingEnemy2::attack(sf::Vector2f playerPosition)
{
    sf::CircleShape bullet(8.0f); // Reducido de 10.0f a 8.0f
    bullet.setFillColor(sf::Color::Blue);
    bullet.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width / 2, sprite.getPosition().y);

    sf::Vector2f direction = playerPosition - bullet.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;

    bullets.push_back(bullet);
    bulletDirections.push_back(direction);
    previousBulletPositions.push_back(bullet.getPosition());
   
}

const std::vector<sf::CircleShape> &ShootingEnemy2::getBullets() const
{
    return bullets;
}

void ShootingEnemy2::storePreviousPosition()
{
    previousPosition = sprite.getPosition();
    previousBulletPositions.clear();
    for (const auto &bullet : bullets)
    {
        previousBulletPositions.push_back(bullet.getPosition());
    }
}

void ShootingEnemy2::drawInterpolated(sf::RenderWindow &window, float alpha)
{
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f interpPos = previousPosition * (1.0f - alpha) + currentPos * alpha;

    sf::Sprite interpSprite = sprite;
    interpSprite.setPosition(interpPos);
    window.draw(interpSprite);

    for (size_t i = 0; i < bullets.size(); ++i)
    {
        sf::Vector2f interpBulletPos = previousBulletPositions[i] * (1.0f - alpha) + bullets[i].getPosition() * alpha;
        sf::CircleShape interpBullet = bullets[i];
        interpBullet.setPosition(interpBulletPos);
        window.draw(interpBullet);
    }
}