#include "ChasingEnemy.h"
#include <cmath>
#include <iostream>

ChasingEnemy::ChasingEnemy(sf::Vector2f position, float speed, TileMap *tileMap)
    : Enemy(position), speed(speed), tileMap(tileMap)
{
    if (!texture.loadFromFile("resources/sprites2.png"))
    {
        std::cerr << "Error cargando la textura del ChasingEnemy" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(4, 2 * 87, 60, 75));
    sprite.setPosition(position);
    previousPosition = position;
}

void ChasingEnemy::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= frameDuration)
    {
        animationTimer = 0.f;
        frameToggle = !frameToggle;
    }

    if (frameToggle)
    {
        sprite.setTextureRect(sf::IntRect(4, 2 * 87, frameWidth, frameHeight));
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(4 + 72, 2 * 87, frameWidth, frameHeight));
    }
}

void ChasingEnemy::update(float deltaTime, sf::Vector2f playerPosition, float mapWidth, float mapHeight)
{
    storePreviousPosition();

    // Calcular dirección hacia el jugador
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length < 1.f) // Evitar moverse si está muy cerca
    {
        updateAnimation(deltaTime);
        return;
    }
    direction /= length;

    // Calcular nueva posición
    sf::Vector2f velocity = direction * speed * deltaTime;
    sf::Vector2f newPos = sprite.getPosition() + velocity;

    // Verificar colisiones con el mapa
    if (tileMap)
    {
        const float tileSize = 32.f;
        sf::FloatRect bounds = sprite.getGlobalBounds();
        bounds.left = newPos.x;
        bounds.top = newPos.y;

        int x1 = static_cast<int>(std::floor(bounds.left / tileSize));
        int y1 = static_cast<int>(std::floor(bounds.top / tileSize));
        int x2 = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
        int y2 = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

        bool walkable = true;
        for (int x = x1; x <= x2 && walkable; ++x)
        {
            for (int y = y1; y <= y2 && walkable; ++y)
            {
                if (x < 0 || x >= static_cast<int>(mapWidth / tileSize) || y < 0 || y >= static_cast<int>(mapHeight / tileSize) || !tileMap->isWalkable(x, y))
                {
                    walkable = false;
                }
            }
        }

        if (walkable)
        {
            sprite.setPosition(newPos);
        }
        else
        {
            // Intentar mover solo en X o Y
            sf::Vector2f newPosX = sprite.getPosition() + sf::Vector2f(velocity.x, 0);
            sf::Vector2f newPosY = sprite.getPosition() + sf::Vector2f(0, velocity.y);

            // Verificar movimiento en X
            bounds.left = newPosX.x;
            bounds.top = newPosX.y;
            x1 = static_cast<int>(std::floor(bounds.left / tileSize));
            y1 = static_cast<int>(std::floor(bounds.top / tileSize));
            x2 = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
            y2 = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

            bool walkableX = true;
            for (int x = x1; x <= x2 && walkableX; ++x)
            {
                for (int y = y1; y <= y2 && walkableX; ++y)
                {
                    if (x < 0 || x >= static_cast<int>(mapWidth / tileSize) || y < 0 || y >= static_cast<int>(mapHeight / tileSize) || !tileMap->isWalkable(x, y))
                    {
                        walkableX = false;
                    }
                }
            }

            // Verificar movimiento en Y
            bounds.left = newPosY.x;
            bounds.top = newPosY.y;
            x1 = static_cast<int>(std::floor(bounds.left / tileSize));
            y1 = static_cast<int>(std::floor(bounds.top / tileSize));
            x2 = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
            y2 = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

            bool walkableY = true;
            for (int x = x1; x <= x2 && walkableY; ++x)
            {
                for (int y = y1; y <= y2 && walkableY; ++y)
                {
                    if (x < 0 || x >= static_cast<int>(mapWidth / tileSize) || y < 0 || y >= static_cast<int>(mapHeight / tileSize) || !tileMap->isWalkable(x, y))
                    {
                        walkableY = false;
                    }
                }
            }

            if (walkableX)
            {
                sprite.setPosition(newPosX);
            }
            else if (walkableY)
            {
                sprite.setPosition(newPosY);
            }
           
        }
    }
    else
    {
        sprite.setPosition(newPos);
        std::cerr << "ChasingEnemy sin TileMap, movido a (" << newPos.x << ", " << newPos.y << ")" << std::endl;
    }

    // Asegurar que el enemigo permanezca dentro de los límites del mapa
    newPos.x = std::max(0.f, std::min(sprite.getPosition().x, mapWidth - sprite.getGlobalBounds().width));
    newPos.y = std::max(0.f, std::min(sprite.getPosition().y, mapHeight - sprite.getGlobalBounds().height));
    sprite.setPosition(newPos);

    updateAnimation(deltaTime);
}

void ChasingEnemy::render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void ChasingEnemy::storePreviousPosition()
{
    previousPosition = sprite.getPosition();
}

void ChasingEnemy::drawInterpolated(sf::RenderWindow &window, float alpha)
{
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f interpPos = previousPosition * (1.0f - alpha) + currentPos * alpha;
    sf::Sprite interpSprite = sprite;
    interpSprite.setPosition(interpPos);
    window.draw(interpSprite);
}