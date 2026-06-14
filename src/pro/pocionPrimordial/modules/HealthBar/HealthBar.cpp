#include "HealthBar.h"
#include <iostream>


HealthBar::HealthBar(int maxHealth, sf::Vector2f position, sf::Vector2f size)
    : maxHealth(maxHealth), currentHealth(maxHealth), size(size)
{

    backgroundBar.setSize(size);
    backgroundBar.setFillColor(sf::Color(50, 50, 50)); // gris oscuro
    backgroundBar.setPosition(position);

    healthBar.setSize(size);
    healthBar.setFillColor(sf::Color(179, 60, 66));
    healthBar.setPosition(position);

    if (!borderTexture.loadFromFile("resources/barravida.png")) {
    std::cerr << "Error cargando barravida.png" << std::endl;
    }
    borderSprite.setTexture(borderTexture);

    // Escalamos el borde para que se ajuste a la barra
    sf::Vector2f borderSize(borderTexture.getSize());
    borderSprite.setScale(
        1.1*size.x / borderSize.x,
        1.5*size.y / borderSize.y
    );

    // Lo colocamos en la misma posición que la barra
    borderSprite.setPosition(position);

    healthBar.move(16, 8);
    backgroundBar.move(16, 8);


}

void HealthBar::takeDamage(int amount)
{
    currentHealth -= amount;
    if (currentHealth < 0)
        currentHealth = 0;
    update();
}

void HealthBar::setHealth(int amount)
{
    currentHealth = amount;
    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
    update();
}

int HealthBar::getHealth() const
{
    return currentHealth;
}

void HealthBar::update()
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBar.setSize(sf::Vector2f(size.x * healthPercent, size.y));
}

void HealthBar::draw(sf::RenderWindow &window)
{
    window.draw(backgroundBar);
    window.draw(healthBar);
    window.draw(borderSprite);
}
