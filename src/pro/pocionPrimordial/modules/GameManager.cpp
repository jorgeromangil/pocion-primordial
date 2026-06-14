// GameManager.cpp

#include "GameManager.h"
#include "States/MenuState.h" // Asegúrate de incluir esta línea
GameManager *GameManager::instance = nullptr;

GameManager::GameManager() : window(sf::VideoMode(960, 720), "La Pocion Primordial")
{
    states.push(std::make_unique<MenuState>(window)); // Inicializa con MenuState
}

GameManager *GameManager::getInstance()
{
    if (!instance)
        instance = new GameManager();
    return instance;
}

void GameManager::pushState(std::unique_ptr<State> newState)
{
    if (newState)
    {
        states.push(std::move(newState));
    }
}

void GameManager::popState()
{
    if (!states.empty())
    {
        states.pop();
    }
}

void GameManager::changeState(std::unique_ptr<State> newState)
{
    if (!states.empty())
    {
        states.pop();
    }
    if (newState)
    {
        states.push(std::move(newState));
    }
}

void GameManager::run()
{
    sf::Clock clock;
    float alpha = 0.0f;      // Inicializamos alpha a 0.0
    float lerpSpeed = 10.0f; // Velocidad de interpolación (ajustar según sea necesario)

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Incrementar alpha según el tiempo transcurrido, pero asegurarse de que no exceda 1
        alpha += lerpSpeed * deltaTime;
        if (alpha > 1.0f)
            alpha = 1.0f;

        if (!states.empty())
        {
            handleInput();
            update(deltaTime);
            render(alpha); // Pasamos alpha al método de renderizado
        }

        // Si necesitas que el alpha vuelva a 0 después de una animación (por ejemplo, para interpolación entre fotogramas)
        if (alpha >= 1.0f)
        {
            alpha = 0.0f; // Reiniciar alpha para el siguiente ciclo
        }
    }
}

void GameManager::handleInput()
{
    states.top()->handleInput(window); // Procesar entrada del estado superior
}

void GameManager::update(float deltaTime)
{
    states.top()->update(deltaTime); // Actualizar el estado superior
}

void GameManager::render(float alpha) // Aquí recibimos el parámetro alpha
{
    window.clear();
    states.top()->render(window, alpha); // Pasamos alpha al estado activo para interpolación
    window.display();
}
