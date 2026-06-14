#include "MenuState.h"
#include "GameManager.h"
#include "PlayingState.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

MenuState::MenuState(sf::RenderWindow &window)
    : menu(window), showingTutorial(false)
{
    // Música
    if (!menuMusic.openFromFile("resources/musica-menu.ogg"))
    {
        std::cerr << "No se pudo cargar la música del menú.\n";
    }
    else
    {
        menuMusic.setLoop(true);
        menuMusic.setVolume(60);
        menuMusic.play();
    }

    // Sonido de navegación
    if (!optionSoundBuffer.loadFromFile("resources/sonido-opcion.ogg"))
    {
        std::cerr << "No se pudo cargar el sonido de opción.\n";
    }
    else
    {
        optionSound.setBuffer(optionSoundBuffer);
        optionSound.setVolume(80);
    }

    // Tutorial
    if (!tutorialTexture.loadFromFile("resources/tutorial.png"))
    {
        std::cerr << "No se pudo cargar la textura del tutorial: resources/tutorial.png\n";
    }
    else
    {
        tutorialSprite.setTexture(tutorialTexture);
        sf::FloatRect bounds = tutorialSprite.getLocalBounds();
        tutorialSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        tutorialSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
        // Escalar para que coincida con el tamaño de la ventana, como creditos.png
        float scaleX = window.getSize().x / bounds.width;
        float scaleY = window.getSize().y / bounds.height;
        float scale = std::min(scaleX, scaleY); // Usar la escala más pequeña para evitar distorsión
        tutorialSprite.setScale(scale, scale);
    }
}

void MenuState::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (showingTutorial)
            {
                // Cualquier tecla transiciona a PlayingState
                optionSound.play();
                GameManager::getInstance()->changeState(std::make_unique<PlayingState>(window, 2));
            }
            else
            {
                // Navegación normal del menú
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                {
                    menu.navigate(event.key.code);
                    optionSound.play();
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    switch (menu.getSelectedOption())
                    {
                    case 0:
                        // Mostrar tutorial
                        showingTutorial = true;
                        optionSound.play();
                        break;
                    case 1:
                        menu.showCredits(window);
                        break;
                    case 2:
                        window.close();
                        break;
                    }
                }
            }
        }
    }
}

void MenuState::update(float deltaTime)
{
    // Sin lógica de actualización necesaria
}

void MenuState::render(sf::RenderWindow &window, float alpha)
{
    window.clear();
    if (showingTutorial)
    {
        window.draw(tutorialSprite);
    }
    else
    {
        menu.draw(window);
    }
    window.display();
}