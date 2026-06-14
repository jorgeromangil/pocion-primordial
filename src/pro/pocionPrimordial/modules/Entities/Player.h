#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player {
public:
    Player();
    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock attackClock;
    float attackInterval; // Tiempo entre ataques
};