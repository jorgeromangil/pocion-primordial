#include "WinState.h"
#include "MenuState.h"
#include "GameManager.h"
#include <iostream>

WinState::WinState(sf::RenderWindow &window)
{
    if (!backgroundTexture.loadFromFile("resources/fin.png"))
    {
        std::cerr << "Error al cargar la imagen de fin de juego!" << std::endl;
        exit(0);
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y);

    if (!font.loadFromFile("resources/AncientModernTales-a7Po.ttf"))
    {
        std::cerr << "Error al cargar la fuente!" << std::endl;
        exit(0);
    }


    subtitleText.setFont(font);
    subtitleText.setString("Has acabado el juego");
    subtitleText.setCharacterSize(80);
    subtitleText.setFillColor(sf::Color(156, 146, 189));

    returnText.setFont(font);
    returnText.setString("Volver al inicio");
    returnText.setCharacterSize(52);
    returnText.setFillColor(sf::Color::White);

    updatePositions(window);
}

void WinState::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            GameManager::getInstance()->changeState(std::make_unique<MenuState>(window));
        }
    }
}

void WinState::update(float)
{
    // No hay lógica de actualización por ahora
}

void WinState::render(sf::RenderWindow &window, float)
{
    window.clear();
    window.draw(backgroundSprite);
    window.draw(subtitleText);
    window.draw(returnText);
    window.display();
}

void WinState::updatePositions(sf::RenderWindow &window)
{
    auto windowSize = window.getSize();


    subtitleText.setPosition(
        windowSize.x / 2 - subtitleText.getGlobalBounds().width / 2,
        windowSize.y / 4 + 80);

    returnText.setPosition(
        windowSize.x / 2 - returnText.getGlobalBounds().width / 2,
        windowSize.y / 2 + 85);
}
