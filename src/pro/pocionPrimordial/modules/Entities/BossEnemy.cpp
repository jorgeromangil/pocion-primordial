#include "BossEnemy.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

BossEnemy::BossEnemy(const sf::Vector2f &position, TileMap *tileMap)
    : Enemy(position), tileMap(tileMap)
{
    if (!texture.loadFromFile("resources/sprites1.png"))
    {
        std::cerr << "Error cargando sprites1.png" << std::endl;
    }
    if (!bulletTexture.loadFromFile("resources/fuego-naranja.png"))
    {
        std::cerr << "Error cargando fuego-naranja.png" << std::endl;
    }
    if (!attackBuffer.loadFromFile("resources/ataque-boss-fuego.ogg"))
    {
        std::cerr << "No se pudo cargar el sonido de ataque del boss.\n";
    }
    attackSound.setBuffer(attackBuffer);
    attackSound.setVolume(60.f);

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(7, 29 + 200, 115, 220)); // Abajo por defecto
    sprite.setPosition(position);
    sprite.setScale(1.2f, 1.2f);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    previousPosition = position;
}

void BossEnemy::update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight)
{
    storePreviousPosition();

    // Mover hacia el jugador
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0)
    {
        direction /= length;
        sf::Vector2f newPos = sprite.getPosition() + direction * speed * deltaTime;
        newPos.x = std::max(0.f, std::min(newPos.x, mapWidth - sprite.getGlobalBounds().width));
        newPos.y = std::max(0.f, std::min(newPos.y, mapHeight - sprite.getGlobalBounds().height));
        sprite.setPosition(newPos);

        // Animación y dirección
        if (std::abs(direction.x) > std::abs(direction.y))
        {
            // Movimiento horizontal
            if (animationClock.getElapsedTime().asSeconds() > 0.2f)
            {
                toggleFrame = !toggleFrame;
                animationClock.restart();
            }

            if (direction.x > 0)
            {
                // Derecha: alterna entre dos frames
                if (toggleFrame)
                    sprite.setTextureRect(sf::IntRect(7, 229, 115, 220)); // frame 1 derecha
                else
                    sprite.setTextureRect(sf::IntRect(135, 229, 115, 220)); // frame 2 derecha
            }
            else
            {
                // Izquierda: alterna entre dos frames
                if (toggleFrame)
                    sprite.setTextureRect(sf::IntRect(263, 229, 115, 220)); // frame 1 izquierda
                else
                    sprite.setTextureRect(sf::IntRect(391, 229, 115, 220)); // frame 2 izquierda
            }
        }
        else
        {
            // Movimiento vertical
            if (direction.y > 0)
                sprite.setTextureRect(sf::IntRect(7, 229, 115, 220)); // Abajo (fijo)
            else
                sprite.setTextureRect(sf::IntRect(523, 229, 115, 220)); // Arriba (fijo)
        }
    }

    // Teletransportación
    teleportTimer += deltaTime;
    if (teleportTimer >= teleportInterval && tileMap)
    {
        sprite.setPosition(tileMap->getRandomWalkablePosition());
        teleportTimer = 0.f;
    }

    // Actualizar balas
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        if (bulletStates[i]) // Proyectil en movimiento
        {
            float angleRad = bulletAngles[i] * 3.14159f / 180.f;
            bullets[i].move(std::cos(angleRad) * bulletSpeeds[i] * deltaTime, std::sin(angleRad) * bulletSpeeds[i] * deltaTime);
            bulletTimers[i] += deltaTime;
            if (bulletTimers[i] >= 1.5f) // Detener tras 1.5s (~300 píxeles)
            {
                bulletStates[i] = false;
                bulletSpeeds[i] = 0.f;
                bulletTimers[i] = 0.f; // Reiniciar temporizador para la fase parada
            }
        }
        else // Proyectil parado
        {
            bulletTimers[i] += deltaTime;
            if (bulletTimers[i] >= 6.0f) // Eliminar tras 10s
            {
                bullets.erase(bullets.begin() + i);
                bulletSpeeds.erase(bulletSpeeds.begin() + i);
                bulletAngles.erase(bulletAngles.begin() + i);
                bulletTimers.erase(bulletTimers.begin() + i);
                bulletStates.erase(bulletStates.begin() + i);
                --i;
                continue;
            }
        }

        // Verificar límites del mapa
        sf::Vector2f pos = bullets[i].getPosition();
        if (pos.x < 0 || pos.x > mapWidth || pos.y < 0 || pos.y > mapHeight)
        {
            bullets.erase(bullets.begin() + i);
            bulletSpeeds.erase(bulletSpeeds.begin() + i);
            bulletAngles.erase(bulletAngles.begin() + i);
            bulletTimers.erase(bulletTimers.begin() + i);
            bulletStates.erase(bulletStates.begin() + i);
            --i;
        }
    }

    // Disparar
    if (attackTimer.getElapsedTime().asSeconds() > attackInterval)
    {
        attack(playerPosition);
    }
}

void BossEnemy::attack(sf::Vector2f playerPosition)
{
    const int numBullets = 6;
    for (int i = 0; i < numBullets; ++i)
    {
        sf::Sprite bullet;
        bullet.setTexture(bulletTexture);
        bullet.setPosition(sprite.getPosition());
        sf::FloatRect bounds = bullet.getLocalBounds();
        bullet.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        bullet.setScale(5.0f, 5.0f);                    // Proyectiles más grandes
        float angle = static_cast<float>(rand() % 360); // Ángulo aleatorio
        bullets.push_back(bullet);
        bulletSpeeds.push_back(200.f); // Velocidad inicial
        bulletAngles.push_back(angle);
        bulletTimers.push_back(0.f);  // Temporizador inicial
        bulletStates.push_back(true); // Comienza moviéndose
    }

    attackTimer.restart();
    attackSound.play();
}

void BossEnemy::render(sf::RenderWindow &window)
{
    window.draw(sprite);
    for (const auto &bullet : bullets)
    {
        window.draw(bullet);
    }
}

void BossEnemy::drawInterpolated(sf::RenderWindow &window, float alpha)
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

void BossEnemy::storePreviousPosition()
{
    previousPosition = sprite.getPosition();
}

const std::vector<sf::Sprite> &BossEnemy::getBullets() const
{
    return bullets;
}

void BossEnemy::takeDamage(float damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

float BossEnemy::getHealth() const
{
    return health;
}