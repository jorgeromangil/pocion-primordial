#include "PlayingState.h"
#include "GameManager.h"
#include "States/PauseState.h"
#include "States/MenuState.h"
#include "KeyboardMovement.h"
#include "Entities/ShootingEnemy.h"
#include "Entities/ChasingEnemy.h"
#include "Entities/BossEnemy.h"
#include "Entities/ShootingEnemy2.h"
#include "Entities/ChasingEnemy2.h"
#include "Entities/BossEnemy2.h"
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "States/EndState.h"
#include "Camera/Camera.h"
#include "Map/TileMap.h"
#include <iterator>
#include <unordered_map>

using std::begin;
using std::end;
std::unordered_map<void *, float> circleWeaponTimers;

PlayingState::PlayingState(sf::RenderWindow &win, int levelIndex)
    : levelIndex(levelIndex), countdownTimer(30.0f), window(win), enemySpawnTimer(3.5f), potionSpawnTimer(5.0f), damageTimer(1.0f),
      isBossPhase(false), shouldExitToMenu(false), showDefeatScreen(false), speedSpinnerRotation(0.f), fireRateSpinnerRotation(0.f),
      messageTimer(0.f), showMessage(false), camera(win)
{
    std::string mapPath;
    std::string tmxPath;

    switch (levelIndex)
    {
    case 1:
        mapPath = "resources/maps/cueva1.png";
        tmxPath = "resources/maps/cueva1.tmx";
        break;
    case 2:
    default:
        mapPath = "resources/maps/cueva2.png";
        tmxPath = "resources/maps/cueva2.tmx";
        break;
    }

    if (!tileMap.load(mapPath, tmxPath))
    {
        std::cerr << "Error cargando el mapa: " << mapPath << std::endl;
    }

    auto movement = std::make_unique<KeyboardMovement>();
    movement->setTileMap(&tileMap);
    player.setMovementStrategy(movement.release());

    if (!defeatFont.loadFromFile("resources/AncientModernTales-a7Po.ttf"))
    {
        std::cerr << "Error cargando AncientModernTales-a7Po.ttf" << std::endl;
    }
    if (!font.loadFromFile("resources/fonts/Arial.ttf"))
    {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    timerText.setFont(defeatFont);
    timerText.setCharacterSize(32);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(15, 620);

    healthBar = std::make_unique<HealthBar>(100, sf::Vector2f(15, 660), sf::Vector2f(200, 30));
    bossHealthBar = std::make_unique<BossHealthBar>(200, sf::Vector2f(230, 40), sf::Vector2f(500, 15));

    enemy = std::make_shared<ShootingEnemy>(sf::Vector2f(400.f, 100.f));

    if (!defeatBackgroundTexture.loadFromFile("resources/recuadro.png"))
    {
        std::cerr << "Error cargando recuadro.png" << std::endl;
    }
    defeatBackgroundSprite.setTexture(defeatBackgroundTexture);
    defeatBackgroundSprite.setScale(0.4f, 0.4f);

    sf::FloatRect bgRect = defeatBackgroundSprite.getLocalBounds();
    defeatBackgroundSprite.setOrigin(bgRect.width / 2.0f, bgRect.height / 2.0f);
    defeatBackgroundSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    defeatText.setFont(defeatFont);
    defeatText.setString("Has perdido");
    defeatText.setCharacterSize(48);
    defeatText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = defeatText.getLocalBounds();
    defeatText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    defeatText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    messageText = defeatText; // Usar el mismo font y configuración inicial
    messageText.setString("");
    messageText.setCharacterSize(32);
    messageText.setFillColor(sf::Color::Red);

    if (!speedPotionTexture.loadFromFile("resources/poc_velocidad.png"))
    {
        std::cerr << "Error cargando POC_VELOCIDAD.png" << std::endl;
    }
    if (!fireRatePotionTexture.loadFromFile("resources/poc_disparo.png"))
    {
        std::cerr << "Error cargando POC_DISPARO.png" << std::endl;
    }
    if (!healthPotionTexture.loadFromFile("resources/poc_vida.png"))
    {
        std::cerr << "Error cargando POC_VIDA.png" << std::endl;
    }

    speedEffectSprite.setTexture(speedPotionTexture);
    speedEffectSprite.setPosition(10.f, 10.f);
    speedEffectSprite.setScale(1.3f, 1.3f);

    fireRateEffectSprite.setTexture(fireRatePotionTexture);
    fireRateEffectSprite.setPosition(68.f, 10.f);
    fireRateEffectSprite.setScale(1.3f, 1.3f);

    healthEffectSprite.setTexture(healthPotionTexture);
    healthEffectSprite.setPosition(126.f, 10.f);
    healthEffectSprite.setScale(1.3f, 1.3f);

    speedEffectCooldown.setPrimitiveType(sf::TriangleFan);
    fireRateEffectCooldown.setPrimitiveType(sf::TriangleFan);
    healthEffectCooldown.setPrimitiveType(sf::TriangleFan);

    if (!healthPotionBuffer.loadFromFile("resources/curacion.ogg"))
        std::cerr << "Error cargando el sonido de la poción de salud" << std::endl;
    healthPotionSound.setBuffer(healthPotionBuffer);

    if (!speedPotionBuffer.loadFromFile("resources/velocidad.ogg"))
        std::cerr << "Error cargando el sonido de la poción de velocidad" << std::endl;
    speedPotionSound.setBuffer(speedPotionBuffer);

    if (!fireRatePotionBuffer.loadFromFile("resources/disp-rapido.ogg"))
        std::cerr << "Error cargando el sonido de la poción de disparo rápido" << std::endl;
    fireRatePotionSound.setBuffer(fireRatePotionBuffer);

    if (!enemydeadbuffer.loadFromFile("resources/ataque-2.ogg"))
    {
        std::cerr << "Error al cargar el sonido de muerte del enemigo" << std::endl;
    }
    enemydeadSound.setBuffer(enemydeadbuffer);
    enemydeadSound.setVolume(100);

    if (!weaponBulletsTexture.loadFromFile("resources/tecla-barita.png"))
    {
        std::cerr << "Error cargando tecla-barita.png" << std::endl;
    }
    else
    {
    }
    weaponBulletsSprite.setTexture(weaponBulletsTexture);
    weaponBulletsSprite.setScale(0.175f, 0.175f);

    if (!weaponCircleTexture.loadFromFile("resources/tecla-alo.png"))
    {
        std::cerr << "Error cargando tecla-alo.png" << std::endl;
    }
    else
    {
    }
    weaponCircleSprite.setTexture(weaponCircleTexture);
    weaponCircleSprite.setScale(0.175f, 0.175f);

    std::string musicPath;
    switch (levelIndex)
    {
    case 1:
        musicPath = "resources/mapa-agua.ogg";
        break;
    case 2:
        musicPath = "resources/mapa-fuego.ogg";
        break;
    default:
        musicPath = "resources/mapa-fuego.ogg";
        break;
    }

    if (!musicPath.empty())
    {
        if (!backgroundMusic.openFromFile(musicPath))
        {
            std::cerr << "Error cargando música: " << musicPath << std::endl;
        }
        else
        {
            backgroundMusic.setLoop(true);
            backgroundMusic.setVolume(100);
            backgroundMusic.play();
        }
    }
}

PlayingState::~PlayingState()
{
}

void PlayingState::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (showDefeatScreen && event.type == sf::Event::KeyPressed)
        {
            GameManager::getInstance()->changeState(std::make_unique<MenuState>(window));
            return;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                GameManager::getInstance()->pushState(std::make_unique<PauseState>(window));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                if (isBossPhase)
                {
                    if (player.getCurrentWeapon() == Character::WeaponType::Bullets)
                    {
                        // Si está en balas y se intenta cambiar a circular, bloquear
                        showMessage = true;
                        messageTimer = 2.0f;
                        messageText.setString("Esa arma no se puede usar");
                        sf::FloatRect messageRect = messageText.getLocalBounds();
                        messageText.setOrigin(messageRect.left + messageRect.width / 2.0f, messageRect.top + messageRect.height / 2.0f);
                        messageText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
                    }
                    // No permitir cambio si ya está en circular (aunque debería estar forzado a balas)
                }
                else
                {
                    player.switchWeapon();
                }
            }
        }
    }
}

void PlayingState::update(float deltaTime)
{
    if (showDefeatScreen)
    {
        return;
    }
    player.updateCircularWeapon();

    countdownTimer.update(deltaTime);
    potionSpawnTimer.update(deltaTime);
    enemySpawnTimer.update(deltaTime);
    damageTimer.update(deltaTime);

    if (showMessage)
    {
        messageTimer -= deltaTime;
        if (messageTimer <= 0.f)
        {
            showMessage = false;
            messageText.setString("");
        }
    }

    timerText.setString("Tiempo restante: " + std::to_string(static_cast<int>(countdownTimer.getTimeRemaining())) + "s");

    static float lastReductionTime = countdownTimer.getTimeRemaining();
    if (static_cast<int>(lastReductionTime / 5) != static_cast<int>(countdownTimer.getTimeRemaining() / 5))
    {
        lastReductionTime = countdownTimer.getTimeRemaining();
        potionSpawnTimer.reduceBaseTime(0.3f);
        enemySpawnTimer.reduceBaseTime(0.5f);
        potionSpawnTimer.reset(potionSpawnTimer.getBaseTime());
        enemySpawnTimer.reset(enemySpawnTimer.getBaseTime());
    }

    if (potionSpawnTimer.isFinished())
    {
        PotionType types[] = {PotionType::Health, PotionType::Speed, PotionType::FireRate};
        PotionType type = types[rand() % 3];
        sf::Vector2f pos = tileMap.getRandomWalkablePosition();
        potions.push_back(std::make_shared<Potion>(type, pos));
        potionSpawnTimer.reset(potionSpawnTimer.getBaseTime());
    }

    for (size_t i = 0; i < potions.size(); ++i)
    {
        sf::FloatRect potionBounds = potions[i]->getBounds();
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
        if (potionBounds.intersects(playerBounds))
        {
            auto type = potions[i]->getType();
            float duration = potions[i]->getEffectDuration();
            float value = potions[i]->getEffectValue();

            switch (type)
            {
            case PotionType::Health:
                healthPotionSound.play();
                break;
            case PotionType::Speed:
                speedPotionSound.play();
                break;
            case PotionType::FireRate:
                fireRatePotionSound.play();
                break;
            }

            switch (type)
            {
            case PotionType::Health:
                player.applyHealthRegen(value, duration);
                break;
            case PotionType::Speed:
                player.applySpeedBoost(value, duration);
                break;
            case PotionType::FireRate:
                player.applyFireRateBoost(value, duration);
                break;
            }
            potions.erase(potions.begin() + i);
            --i;
        }
    }

    player.updateEffects(deltaTime);
    healthBar->setHealth(player.getHealth());

    if (player.isSpeedBoostActive())
    {
        float percent = player.getSpeedBoostTimeRemaining() / 7.f;
        createCooldownArc(speedEffectCooldown, percent, sf::Vector2f(30.f, 29.f), 25.f, sf::Color(0, 0, 255, 200));
    }

    if (player.isFireRateBoostActive())
    {
        float percent = player.getFireRateBoostTimeRemaining() / 7.f;
        createCooldownArc(fireRateEffectCooldown, percent, sf::Vector2f(90.f, 29.f), 25.f, sf::Color(0, 255, 0, 200));
    }

    if (player.isHealthRegenActive())
    {
        float percent = player.getHealthRegenTimeRemaining() / 7.f;
        createCooldownArc(healthEffectCooldown, percent, sf::Vector2f(146.f, 29.f), 25.f, sf::Color(255, 0, 0, 200));
    }

    if (!isBossPhase && enemySpawnTimer.isFinished())
    {
        sf::Vector2f randomPosition = tileMap.getRandomWalkablePosition();
        int randomNumber = rand() % 10;

        if (levelIndex == 2)
        {
            if (randomNumber % 2 == 0)
                enemies.push_back(std::make_shared<ShootingEnemy2>(randomPosition));
            else
                enemies.push_back(std::make_shared<ChasingEnemy2>(randomPosition, 80.0f, &tileMap));
        }
        else
        {
            if (randomNumber % 2 == 0)
                enemies.push_back(std::make_shared<ShootingEnemy>(randomPosition));
            else
                enemies.push_back(std::make_shared<ChasingEnemy>(randomPosition, 80.0f, &tileMap));
        }

        enemySpawnTimer.reset(enemySpawnTimer.getBaseTime());
    }

    if (countdownTimer.isFinished() && !isBossPhase)
    {
        isBossPhase = true;
        enemies.clear();
        enemySpawnTimer.reset(9999.f);
        if (levelIndex == 2)
        {
            boss = std::make_shared<BossEnemy2>(tileMap.getRandomWalkablePosition(), &tileMap);
        }
        else
        {
            boss = std::make_shared<BossEnemy>(tileMap.getRandomWalkablePosition(), &tileMap);
        }
        enemies.push_back(boss);
        // Forzar el arma a balas (varita) al iniciar la fase de boss
        if (player.getCurrentWeapon() == Character::WeaponType::Circular)
        {
            player.switchWeapon();
        }
        return;
    }

    if (isBossPhase)
    {
        timerText.setString("Acaba con el jefe");
    }

    player.shoot();
    player.updateBullets(deltaTime, tileMap);

    for (auto &enemy : enemies)
    {
        enemy->update(deltaTime, player.getSprite().getPosition(), tileMap.getMapWidth() * tileMap.getTileWidth(), tileMap.getMapHeight() * tileMap.getTileHeight());

        sf::FloatRect enemyBounds = enemy->getBounds();
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
        if (enemyBounds.intersects(playerBounds))
        {
            if (damageTimer.isFinished())
            {
                player.takeDamage(10);
                healthBar->setHealth(player.getHealth());
                damageTimer.reset(1.0f);
            }
        }

        auto shootingEnemy = std::dynamic_pointer_cast<ShootingEnemy>(enemy);
        if (shootingEnemy)
        {
            for (const auto &bullet : shootingEnemy->getBullets())
            {
                sf::FloatRect bulletBounds = bullet.getGlobalBounds();
                if (bulletBounds.intersects(playerBounds))
                {
                    if (damageTimer.isFinished())
                    {
                        player.takeDamage(10);
                        healthBar->setHealth(player.getHealth());
                        damageTimer.reset(1.0f);
                    }
                    break;
                }
            }
        }

        auto bossEnemy = std::dynamic_pointer_cast<BossEnemy>(enemy);
        if (bossEnemy)
        {
            for (const auto &bullet : bossEnemy->getBullets())
            {
                sf::FloatRect bulletBounds = bullet.getGlobalBounds();
                if (bulletBounds.intersects(playerBounds))
                {
                    if (damageTimer.isFinished())
                    {
                        player.takeDamage(15);
                        healthBar->setHealth(player.getHealth());
                        damageTimer.reset(1.0f);
                    }
                    break;
                }
            }
            bossHealthBar->setHealth(bossEnemy->getHealth());
        }

        auto shootingEnemy2 = std::dynamic_pointer_cast<ShootingEnemy2>(enemy);
        if (shootingEnemy2)
        {
            for (const auto &bullet : shootingEnemy2->getBullets())
            {
                if (bullet.getGlobalBounds().intersects(playerBounds))
                {
                    if (damageTimer.isFinished())
                    {
                        player.takeDamage(10);
                        healthBar->setHealth(player.getHealth());
                        damageTimer.reset(1.0f);
                    }
                    break;
                }
            }
        }

        auto bossEnemy2 = std::dynamic_pointer_cast<BossEnemy2>(enemy);
        if (bossEnemy2)
        {
            for (const auto &bullet : bossEnemy2->getBullets())
            {
                if (bullet.getGlobalBounds().intersects(playerBounds))
                {
                    if (damageTimer.isFinished())
                    {
                        player.takeDamage(15);
                        healthBar->setHealth(player.getHealth());
                        damageTimer.reset(1.0f);
                    }
                    break;
                }
            }
        }
    }

    for (size_t i = 0; i < player.getMutableBullets().size(); ++i)
    {
        const auto &bullet = player.getMutableBullets()[i];
        for (size_t j = 0; j < enemies.size(); ++j)
        {
            if (bullet.getGlobalBounds().intersects(enemies[j]->getBounds()))
            {
                auto bossEnemy = std::dynamic_pointer_cast<BossEnemy>(enemies[j]);
                if (bossEnemy)
                {
                    bossEnemy->takeDamage(10);
                    if (bossEnemy->getHealth() <= 0)
                    {
                        GameManager::getInstance()->changeState(std::make_unique<EndState>(window, levelIndex));
                        return;
                    }
                }
                else
                {
                    enemies.erase(enemies.begin() + j);
                }
                player.removeBullet(i);
                --i;
                break;
            }
        }
    }

    if (player.getCurrentWeapon() == Character::WeaponType::Circular)
    {
        const sf::CircleShape &circle = player.getCircularWeapon();
        sf::Vector2f center = circle.getPosition();
        float reducedRadius = circle.getRadius();

        sf::FloatRect reducedBounds(
            center.x - reducedRadius,
            center.y - reducedRadius,
            reducedRadius * 2.f,
            reducedRadius * 2.f);

        for (int j = enemies.size() - 1; j >= 0; --j)
        {
            if (std::dynamic_pointer_cast<BossEnemy>(enemies[j]) || std::dynamic_pointer_cast<BossEnemy2>(enemies[j]))
            {
                continue;
            }

            void *enemyPtr = static_cast<void *>(enemies[j].get());
            if (reducedBounds.intersects(enemies[j]->getBounds()))
            {
                circleWeaponTimers[enemyPtr] += deltaTime;
                if (circleWeaponTimers[enemyPtr] >= 1.0f)
                {
                    enemies.erase(enemies.begin() + j);
                    enemydeadSound.play();
                    circleWeaponTimers.erase(enemyPtr);
                }
            }
            else
            {
                circleWeaponTimers[enemyPtr] = 0.f;
            }
        }
    }

    if (player.getHealth() <= 0 && !shouldExitToMenu)
    {
        std::cerr << "La vida llegó a 0. Marcando para salir al menú..." << std::endl;
        shouldExitToMenu = true;
        showDefeatScreen = true;
        return;
    }

    player.move(deltaTime);
    player.updateAnimation(deltaTime);
    camera.update(player.getSprite().getPosition());
}

void PlayingState::render(sf::RenderWindow &window, float alpha)
{
    camera.apply();
    window.clear();

    window.draw(tileMap);
    player.drawInterpolated(window, alpha);
    player.renderBullets(window);

    for (const auto &enemy : enemies)
    {
        enemy->drawInterpolated(window, alpha);
    }

    for (const auto &potion : potions)
    {
        sf::FloatRect bounds = potion->getBounds();
        potion->render(window);
    }

    window.setView(window.getDefaultView());
    window.draw(timerText);
    healthBar->draw(window);
    if (isBossPhase)
    {
        bossHealthBar->draw(window);
    }

    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;
    float offsetX = 380.f;
    float offsetY = 280.f;
    sf::Vector2f weaponPosition(centerX + offsetX, centerY + offsetY);

    sf::FloatRect bulletBounds = weaponBulletsSprite.getLocalBounds();
    weaponBulletsSprite.setOrigin(bulletBounds.width / 2.0f, bulletBounds.height / 2.0f);
    sf::FloatRect circleBounds = weaponCircleSprite.getLocalBounds();
    weaponCircleSprite.setOrigin(circleBounds.width / 2.0f, circleBounds.height / 2.0f);

    weaponBulletsSprite.setPosition(weaponPosition);
    weaponCircleSprite.setPosition(weaponPosition);

    if (player.getCurrentWeapon() == Character::WeaponType::Bullets)
    {
        window.draw(weaponBulletsSprite);
    }
    else
    {
        window.draw(weaponCircleSprite);
    }

    if (player.isSpeedBoostActive())
    {
        window.draw(speedEffectCooldown);
        window.draw(speedEffectSprite);
    }

    if (player.isFireRateBoostActive())
    {
        window.draw(fireRateEffectCooldown);
        window.draw(fireRateEffectSprite);
    }

    if (player.isHealthRegenActive())
    {
        window.draw(healthEffectCooldown);
        window.draw(healthEffectSprite);
    }

    if (showDefeatScreen)
    {
        window.draw(defeatBackgroundSprite);
        window.draw(defeatText);
    }

    if (showMessage)
    {
        window.draw(messageText);
    }

    window.display();
}

sf::Vector2f PlayingState::getRandomWalkablePosition([[maybe_unused]] bool isBoss)
{
    const float tileSize = 32.f;
    const int mapTileWidth = tileMap.getMapWidth();
    const int mapTileHeight = tileMap.getMapHeight();
    const sf::FloatRect enemyBounds(0, 0, 60, 75);

    for (int attempts = 0; attempts < 100; ++attempts)
    {
        sf::Vector2f pos = tileMap.getRandomWalkablePosition();
        sf::FloatRect bounds(pos.x, pos.y, enemyBounds.width, enemyBounds.height);

        int x1 = static_cast<int>(std::floor(bounds.left / tileSize));
        int y1 = static_cast<int>(std::floor(bounds.top / tileSize));
        int x2 = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
        int y2 = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

        bool walkable = true;
        for (int x = x1; x <= x2; ++x)
        {
            for (int y = y1; y <= y2; ++y)
            {
                if (x < 0 || x >= mapTileWidth || y < 0 || y >= mapTileHeight || !tileMap.isWalkable(x, y))
                {
                    walkable = false;
                    break;
                }
            }
            if (!walkable)
                break;
        }

        if (walkable)
        {
            return pos;
        }
    }

    return sf::Vector2f(960.f, 460.f);
}

void PlayingState::createCooldownArc(sf::VertexArray &vertexArray, float percent, sf::Vector2f center, float radius, sf::Color color)
{
    const int maxPoints = 60;
    int points = static_cast<int>(std::ceil(percent * maxPoints));

    vertexArray.clear();
    vertexArray.append(sf::Vertex(center, color));

    for (int i = 0; i <= points; ++i)
    {
        float angle = -90.f + (360.f * (i / static_cast<float>(maxPoints)) * percent);
        float rad = angle * 3.14159265f / 180.f;
        float x = center.x + std::cos(rad) * radius;
        float y = center.y + std::sin(rad) * radius;
        vertexArray.append(sf::Vertex(sf::Vector2f(x, y), color));
    }
}

float PlayingState::getDistance(const sf::Vector2f &a, const sf::Vector2f &b)
{
    sf::Vector2f diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}