#include "PauseState.h"
#include "GameManager.h"
#include <SFML/Graphics.hpp>
#include <iostream> // Añadir esta línea para usar std::cerr

PauseState::PauseState(sf::RenderWindow &window)
    : selectedOption(0)
{
    // Cargar la textura de fondo
    if (!backgroundTexture.loadFromFile("resources/pausa.png"))
    {
        std::cerr << "Error al cargar la textura de fondo" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );
    

    // Cargar la fuente
    if (!font.loadFromFile("resources/AncientModernTales-a7Po.ttf"))
    {
        std::cerr << "Error cargando la fuente" << std::endl;
    }

    // Configurar las opciones del menú
    options[0] = "Continuar";
    options[1] = "Salir";

    // Configurar el texto de las opciones
    for (int i = 0; i < 2; ++i)
    {
        text[i].setFont(font);
        text[i].setString(options[i]);
        text[i].setCharacterSize(50);
        text[i].setFillColor(i == selectedOption ? sf::Color::White : sf::Color(105, 77, 126));
        text[i].setPosition(window.getSize().x / 2.0f - text[i].getLocalBounds().width / 2.0f,
                            window.getSize().y / 2.0f + i * OPTION_HEIGHT - text[i].getLocalBounds().height / 2.0f);
    }
    // Cargar la música del menú
    if (!menuMusic.openFromFile("resources/musica-menu.ogg")) {
        std::cerr << "Error al cargar la música del menú" << std::endl;
    }
    menuMusic.setLoop(true);  // Repetir la música
    menuMusic.play();  // Reproducir la música

    // Cargar el sonido de la opción
    if (!optionSoundBuffer.loadFromFile("resources/sonido-opcion.ogg")) {
        std::cerr << "Error al cargar el sonido de la opción" << std::endl;
    }
    optionSound.setBuffer(optionSoundBuffer);  
}

void PauseState::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::W)
            {
                if (selectedOption > 0)
                {
                    selectedOption--;
                    optionSound.play();  // Reproducir sonido al moverse hacia arriba
                }
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                if (selectedOption < 1)
                {
                    selectedOption++;
                    optionSound.play();  // Reproducir sonido al moverse hacia abajo
                }
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (selectedOption == 0) // Continuar
                {
                    GameManager::getInstance()->popState();
                }
                else if (selectedOption == 1) // Salir
                {
                    window.close();
                }
            }

            // Actualizar el color de las opciones
            for (int i = 0; i < 2; ++i)
            {
                text[i].setFillColor(i == selectedOption ? sf::Color::White : sf::Color(105, 77, 126));
            }
        }
    }
}


void PauseState::update(float deltaTime)
{
    // Si deltaTime no se usa, marcamos el parámetro como "no utilizado" para evitar advertencias.
    (void)deltaTime; // Eliminar advertencias por parámetro no utilizado
}

void PauseState::render(sf::RenderWindow &window, float alpha)
{
    // Si el parámetro alpha no se usa, lo marcamos como no utilizado
    (void)alpha;

    window.clear();
    window.draw(backgroundSprite);

    // Dibujar las opciones de pausa
    for (int i = 0; i < 2; ++i)
    {
        window.draw(text[i]);
    }

    window.display();
}
