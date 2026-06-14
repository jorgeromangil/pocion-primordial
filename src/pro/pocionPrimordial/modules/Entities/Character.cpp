#include "Character.h"
#include <iostream>

Character::Character()
{
    if (!texture.loadFromFile("resources/sprites1.png"))
    {
        std::cerr << "Error cargando la imagen sprites1.png" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(7, 14, 110, 200));
    sprite.setPosition(960, 460);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height);

    initializeBulletShape();
    initializeCircularWeapon();

    if (!attackSoundBuffer.loadFromFile("resources/bruja-ataque1.ogg"))
    {
        std::cerr << "Error cargando el sonido de ataque de la bruja" << std::endl;
    }
    attackSound.setBuffer(attackSoundBuffer);
}

void Character::setMovementStrategy(IMovementStrategy *strategy)
{
    movementStrategy = strategy;
}

void Character::move(float deltaTime)
{
    if (movementStrategy)
    {
        previousPosition = sprite.getPosition();
        movementStrategy->move(sprite, deltaTime);
    }
}

void Character::drawInterpolated(sf::RenderWindow &window, float alpha)
{
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f interpolatedPos = previousPosition * (1.0f - alpha) + currentPos * alpha;

    sf::Sprite interpolatedSprite = sprite;
    interpolatedSprite.setPosition(interpolatedPos);

    // Ajustar la posición vertical del círculo para centrarlo
    sf::Vector2f offset(0.f, -sprite.getGlobalBounds().height / 2.f);
    circularWeapon.setPosition(interpolatedPos + offset);

    if (currentWeapon == WeaponType::Circular)
    {
        sf::Vector2f offset(0.f, -sprite.getGlobalBounds().height / 2.f);
        circularWeapon.setPosition(interpolatedPos + offset);
        window.draw(circularWeapon);
    }

    window.draw(interpolatedSprite);
    window.draw(circularWeapon);
}

void Character::updateAnimation(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration)
        {
            animationTimer = 0.f;
            frame = (frame == 0) ? 1 : 0;
        }

        int col = (frame == 0) ? 0 : 1;
        sprite.setTextureRect(sf::IntRect(7 + col * 127, 14, 110, 200));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration)
        {
            animationTimer = 0.f;
            frame = (frame == 0) ? 1 : 0;
        }

        int col = (frame == 0) ? 2 : 3;
        sprite.setTextureRect(sf::IntRect(col * 134, 14, 110, 200));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        sprite.setTextureRect(sf::IntRect(4 * 133, 14, 110, 200));
        frame = 0;
        animationTimer = 0.f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        sprite.setTextureRect(sf::IntRect(7, 14, 110, 200));
        frame = 0;
        animationTimer = 0.f;
    }
    else
    {
        frame = 0;
        animationTimer = 0.f;
    }
}

sf::Sprite Character::getSprite() const
{
    return sprite;
}

void Character::initializeBulletShape()
{
    bulletTemplate.setPointCount(8);

    float radius1 = 10.f;
    float radius2 = 5.f;
    float angle = 0;
    float step = 3.14159f / 4.f;

    for (int i = 0; i < 8; ++i)
    {
        float r = (i % 2 == 0) ? radius1 : radius2;
        sf::Vector2f point(std::cos(angle) * r, std::sin(angle) * r);
        bulletTemplate.setPoint(i, point);
        angle += step;
    }

    bulletTemplate.setFillColor(sf::Color(111, 46, 158));
}

void Character::shoot()
{
    if (currentWeapon != WeaponType::Bullets)
        return;

    if (shootTimer.getElapsedTime().asSeconds() > shootCooldown)
    {
        sf::Vector2f directions[] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

        for (const auto &dir : directions)
        {
            sf::ConvexShape bullet = bulletTemplate;
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sf::Vector2f shootOrigin = {
                bounds.left + bounds.width / 2.f,
                bounds.top + bounds.height / 2.f};
            bullet.setPosition(shootOrigin);
            bullets.push_back(bullet);
            bulletDirections.push_back(dir);
        }
        attackSound.play();

        shootTimer.restart();
    }
}

void Character::updateBullets(float deltaTime, const TileMap &tileMap)
{

    if (currentWeapon != WeaponType::Bullets)
        return;

    for (int i = bullets.size() - 1; i >= 0; --i)
    {
        bullets[i].move(bulletDirections[i] * bulletSpeed * deltaTime);

        int mapWidthPixels = tileMap.getTileWidth() * tileMap.getMapWidth();
        int mapHeightPixels = tileMap.getTileHeight() * tileMap.getMapHeight();

        sf::Vector2f bulletPos = bullets[i].getPosition();
        if (bulletPos.x < 0 || bulletPos.y < 0 || bulletPos.x > mapWidthPixels || bulletPos.y > mapHeightPixels)
        {
            removeBullet(i);
        }
    }
}

void Character::removeBullet(size_t index)
{
    if (index < bullets.size())
    {
        bullets.erase(bullets.begin() + index);
        bulletDirections.erase(bulletDirections.begin() + index);
    }
}

void Character::renderBullets(sf::RenderWindow &window)
{
    for (const auto &bullet : bullets)
    {
        window.draw(bullet);
    }
}

std::vector<sf::ConvexShape> &Character::getMutableBullets()
{
    return bullets;
}

void Character::takeDamage(float damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

void Character::heal(float amount)
{
    health += amount;
    if (health > maxHealth)
        health = maxHealth;
}

void Character::applySpeedBoost(float multiplier, float duration)
{
    if (movementStrategy)
    {
        movementStrategy->setSpeedMultiplier(multiplier);
        speedBoostTimer.restart();
        speedBoostDuration = duration;
    }
}

void Character::applyFireRateBoost(float multiplier, float duration)
{
    shootCooldown = baseShootCooldown * multiplier;
    fireRateBoostTimer.restart();
    fireRateBoostDuration = duration;
}

void Character::applyHealthRegen(float value, float duration)
{
    healthRegenValue = value;
    healthRegenDuration = duration;
    healthRegenTimer.restart();
}

void Character::updateEffects(float deltaTime)
{
    if (speedBoostDuration > 0.f && speedBoostTimer.getElapsedTime().asSeconds() > speedBoostDuration)
    {
        if (movementStrategy)
        {
            movementStrategy->setSpeedMultiplier(1.f);
        }
        speedBoostDuration = 0.f;
    }

    if (fireRateBoostDuration > 0.f && fireRateBoostTimer.getElapsedTime().asSeconds() > fireRateBoostDuration)
    {
        shootCooldown = baseShootCooldown;
        fireRateBoostDuration = 0.f;
    }
    if (healthRegenDuration > 0.f && healthRegenTimer.getElapsedTime().asSeconds() <= healthRegenDuration)
    {
        float regenAmount = (healthRegenValue * deltaTime) / healthRegenDuration;
        heal(regenAmount);
    }
    else if (healthRegenDuration > 0.f)
    {
        healthRegenValue = 0.f;
        healthRegenDuration = 0.f;
    }
}

void Character::initializeCircularWeapon()
{
    circularWeapon.setRadius(circularWeaponRadius);
    circularWeapon.setOrigin(circularWeaponRadius, circularWeaponRadius); // Centrar el círculo
    circularWeapon.setFillColor(sf::Color::Transparent);                  // Sin relleno
    circularWeapon.setOutlineColor(sf::Color(144, 238, 144));             // Verde claro
    circularWeapon.setOutlineThickness(3.f);                              // Grosor del borde
    circularWeapon.setPosition(sprite.getPosition());                     // Inicialmente alrededor del personaje
}

void Character::updateCircularWeapon()
{
    if (currentWeapon != WeaponType::Circular)
        return;
    circularWeapon.setPosition(sprite.getPosition());
}

void Character::switchWeapon()
{
    if (currentWeapon == WeaponType::Bullets)
    {
        currentWeapon = WeaponType::Circular;
        bullets.clear();
        bulletDirections.clear();
    }
    else
    {
        currentWeapon = WeaponType::Bullets;
    }
    if (currentWeapon == WeaponType::Circular)
    {
        circularWeapon.setFillColor(sf::Color::Transparent);
        circularWeapon.setOutlineColor(sf::Color(144, 238, 144));
    }
    else
    {
        circularWeapon.setFillColor(sf::Color(0, 0, 0, 0));
        circularWeapon.setOutlineColor(sf::Color(0, 0, 0, 0));
    }
}

void Character::draw(sf::RenderWindow &window)
{
    if (currentWeapon == WeaponType::Circular)
    {
        window.draw(circularWeapon);
    }
    else
    {
        renderBullets(window);
    }

    window.draw(sprite);
}

const sf::CircleShape &Character::getCircularWeapon() const
{
    return circularWeapon;
}
