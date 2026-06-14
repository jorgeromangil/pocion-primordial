#ifndef POTION_H
#define POTION_H

#include <SFML/Graphics.hpp>

enum class PotionType
{
    Health,
    Speed,
    FireRate
};

class Potion
{
public:
    Potion(PotionType type, sf::Vector2f position);
    void render(sf::RenderWindow &window);
    sf::FloatRect getBounds() const;
    PotionType getType() const;
    float getEffectDuration() const;
    float getEffectValue() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    PotionType type;
    float effectDuration;
    float effectValue;
};

#endif