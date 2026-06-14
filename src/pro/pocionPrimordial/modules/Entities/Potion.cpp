#include "Potion.h"
#include <iostream>

Potion::Potion(PotionType type, sf::Vector2f position)
    : type(type)
{
    std::string textureFile;
    switch (type)
    {
    case PotionType::Health:
        textureFile = "resources/poc_vida.png";
        effectValue = 70.f;   // Regenera 70 puntos (70% de maxHealth) durante 7 segundos
        effectDuration = 7.f; // Duración de regeneración
        break;
    case PotionType::Speed:
        textureFile = "resources/poc_velocidad.png";
        effectValue = 1.5f;
        effectDuration = 7.f;
        break;
    case PotionType::FireRate:
        textureFile = "resources/poc_disparo.png";
        effectValue = 0.5f;
        effectDuration = 7.f;
        break;
    }

    if (!texture.loadFromFile(textureFile))
    {
        std::cerr << "Error cargando la textura " << textureFile << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setScale(2.0f, 2.0f);
}

void Potion::render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

sf::FloatRect Potion::getBounds() const
{
    return sprite.getGlobalBounds();
}

PotionType Potion::getType() const
{
    return type;
}

float Potion::getEffectDuration() const
{
    return effectDuration;
}

float Potion::getEffectValue() const
{
    return effectValue;
}