#include "BossHealthBar.h"
#include <iostream>

BossHealthBar::BossHealthBar(int maxHealth, sf::Vector2f position, sf::Vector2f size)
    : maxHealth(maxHealth), currentHealth(maxHealth), size(size)
{
    // Barra de fondo
    backgroundBar.setSize(size);
    backgroundBar.setFillColor(sf::Color(50, 50, 50)); 
    backgroundBar.setPosition(position);

    // Barra de vida 
    healthBar.setSize(size);
    healthBar.setFillColor(sf::Color(137, 109, 158));
    healthBar.setPosition(position);

    // Borde decorativo (opcional)
    if (!borderTexture.loadFromFile("resources/barravidaboss.png")) {
        std::cerr << "Error cargando barravida.png" << std::endl;
    }
    borderSprite.setTexture(borderTexture);

    sf::Vector2f borderSize(borderTexture.getSize());
    borderSprite.setScale(
        1.04f * size.x / borderSize.x,
        1.1f * size.y / borderSize.y
    );
    borderSprite.setPosition(position);

    borderSprite.move(-12, 0);

    // Fuente y título
    if (!font.loadFromFile("resources/AncientModernTales-a7Po.ttf")) {
        std::cerr << "Error cargando fuente AncientModernTales-a7Po.ttf" << std::endl;
    }

    title.setFont(font);
    title.setString("Vida del jefe");
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::White);
    // Centrar el texto respecto al ancho del texto
    sf::FloatRect textBounds = title.getLocalBounds();
    title.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);

    // Posicionarlo en el centro horizontal de la barra
    title.setPosition(position.x + size.x / 2.f, position.y - 20.f);
}

void BossHealthBar::takeDamage(int amount)
{
    currentHealth -= amount;
    if (currentHealth < 0)
        currentHealth = 0;
    update();
}

void BossHealthBar::setHealth(int amount)
{
    currentHealth = amount;
    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
    update();
}

int BossHealthBar::getHealth() const
{
    return currentHealth;
}

void BossHealthBar::update()
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBar.setSize(sf::Vector2f(size.x * healthPercent, size.y));
}

void BossHealthBar::draw(sf::RenderWindow &window)
{
    window.draw(backgroundBar);
    window.draw(healthBar);
    window.draw(borderSprite);
    window.draw(title);
}
