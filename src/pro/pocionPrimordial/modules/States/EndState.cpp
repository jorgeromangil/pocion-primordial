#include "EndState.h"
#include "WinState.h"
#include "MenuState.h"
#include "GameManager.h"
#include "PlayingState.h" // o el estado del siguiente nivel

#include <iostream>

EndState::EndState(sf::RenderWindow &window, int currentLevel)
    : currentLevel(currentLevel)
{
    if (!backgroundTexture.loadFromFile("resources/finnivel.png"))
    {
        std::cerr << "Error al cargar la imagen de fin de nivel!" << std::endl;
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

    titleText.setFont(font);
    titleText.setString("Enhorabuena");
    titleText.setCharacterSize(130);
    titleText.setFillColor(sf::Color(156, 146, 189));

    subtitleText.setFont(font);
    subtitleText.setString("Has completado el nivel");
    subtitleText.setCharacterSize(80);
    subtitleText.setFillColor(sf::Color(156, 146, 189));

    nextLevelText.setFont(font);
    nextLevelText.setString("Siguiente nivel");
    nextLevelText.setCharacterSize(52);
    nextLevelText.setFillColor(sf::Color::White);

    updatePositions(window);
}

void EndState::handleInput(sf::RenderWindow &window)
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
            int nextLevel = currentLevel - 1; // Vamos de cueva2 (2) → cueva1 (1)
            if (nextLevel >= 1) {
                GameManager::getInstance()->changeState(std::make_unique<PlayingState>(window, nextLevel));
            } else {
                // No hay más niveles → volver al menú, o mensaje de "fin del juego"
                GameManager::getInstance()->changeState(std::make_unique<WinState>(window));
            }

        }
    }
}

void EndState::update(float deltaTime)
{
    // Nada por ahora
}

// EndState.cpp
void EndState::render(sf::RenderWindow &window, float alpha)
{
    window.clear();

  

    // Dibujar los elementos
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(nextLevelText);

    window.display();
}

void EndState::updatePositions(sf::RenderWindow &window)
{
    sf::Vector2u windowSize = window.getSize();

    titleText.setPosition(
        windowSize.x / 2 - titleText.getGlobalBounds().width / 2,
        windowSize.y / 5);

    subtitleText.setPosition(
        windowSize.x / 2 - subtitleText.getGlobalBounds().width / 2,
        windowSize.y / 4 + 80);

    nextLevelText.setPosition(
        windowSize.x / 2 - nextLevelText.getGlobalBounds().width / 2,
        windowSize.y / 2 + 85);
}
