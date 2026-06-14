#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "State.h"
#include "../Map/TileMap.h"
#include "../Entities/Character.h"
#include "../Entities/Enemy.h"
#include "../Entities/BossEnemy.h"
#include "../HealthBar/HealthBar.h"
#include "../HealthBar/BossHealthBar.h"
#include "../Timer/Timer.h"
#include "../Camera/Camera.h"
#include "../Entities/Potion.h"
#include <memory>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class PlayingState : public State
{
public:
    PlayingState(sf::RenderWindow &window, int levelIndex);
    ~PlayingState();
    void handleInput(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window, float alpha) override;

private:
    void createCooldownArc(sf::VertexArray &vertexArray, float percent, sf::Vector2f center, float radius, sf::Color color);
    sf::Vector2f getRandomWalkablePosition([[maybe_unused]] bool isBoss);

    int levelIndex;
    Timer countdownTimer;
    sf::RenderWindow &window;
    Timer enemySpawnTimer;
    Timer potionSpawnTimer;
    Timer damageTimer;
    bool isBossPhase;
    bool shouldExitToMenu;
    bool showDefeatScreen;
    float speedSpinnerRotation;
    float fireRateSpinnerRotation;
    float messageTimer; // Nuevo: Temporizador para el mensaje
    bool showMessage;   // Nuevo: Indicador para mostrar el mensaje
    Camera camera;
    TileMap tileMap;
    Character player;
    std::shared_ptr<Enemy> enemy;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::shared_ptr<BossEnemy> boss;
    std::vector<std::shared_ptr<Potion>> potions;
    sf::Font font;
    sf::Font defeatFont;
    sf::Text timerText;
    sf::Text defeatText;
    sf::Text messageText; // Nuevo: Texto para el mensaje
    std::unique_ptr<HealthBar> healthBar;
    std::unique_ptr<BossHealthBar> bossHealthBar;
    sf::Texture defeatBackgroundTexture;
    sf::Sprite defeatBackgroundSprite;
    sf::Texture speedPotionTexture;
    sf::Texture fireRatePotionTexture;
    sf::Texture healthPotionTexture;
    sf::Sprite speedEffectSprite;
    sf::Sprite fireRateEffectSprite;
    sf::Sprite healthEffectSprite;
    sf::VertexArray speedEffectCooldown;
    sf::VertexArray fireRateEffectCooldown;
    sf::VertexArray healthEffectCooldown;
    sf::RectangleShape speedEffectIndicator;
    sf::RectangleShape fireRateEffectIndicator;
    sf::CircleShape speedEffectSpinner;
    sf::CircleShape fireRateEffectSpinner;
    sf::SoundBuffer healthPotionBuffer;
    sf::Sound healthPotionSound;
    sf::SoundBuffer speedPotionBuffer;
    sf::Sound speedPotionSound;
    sf::SoundBuffer fireRatePotionBuffer;
    sf::Sound fireRatePotionSound;
    sf::SoundBuffer enemydeadbuffer;
    sf::Sound enemydeadSound;
    sf::Music backgroundMusic;
    sf::Texture weaponBulletsTexture;
    sf::Texture weaponCircleTexture;
    sf::Sprite weaponBulletsSprite;
    sf::Sprite weaponCircleSprite;

    float getDistance(const sf::Vector2f &a, const sf::Vector2f &b);
};

#endif