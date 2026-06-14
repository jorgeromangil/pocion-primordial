#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>

class HealthBar
{
private:
    int maxHealth;
    int currentHealth;
    sf::Vector2f size;
    sf::RectangleShape backgroundBar;
    sf::RectangleShape healthBar;
    sf::Texture borderTexture;
    sf::Sprite borderSprite;


public:
    HealthBar(int maxHealth, sf::Vector2f position, sf::Vector2f size);

    void takeDamage(int amount);
    void setHealth(int amount);
    int getHealth() const;

    void update();
    void draw(sf::RenderWindow &window);
};

#endif // HEALTHBAR_H
