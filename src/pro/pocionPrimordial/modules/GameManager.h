// GameManager.h

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "States/State.h"

class GameManager
{
public:
    static GameManager *getInstance();
    void pushState(std::unique_ptr<State> newState);   // Apilar un nuevo estado
    void popState();                                   // Desapilar el estado superior
    void changeState(std::unique_ptr<State> newState); // Reemplazar el estado superior
    void run();

private:
    GameManager();
    static GameManager *instance;
    std::stack<std::unique_ptr<State>> states; // Pila de estados
    sf::RenderWindow window;

    void handleInput();
    void update(float deltaTime);
    void render(float alpha); // Cambié aquí para que acepte alpha
};

#endif
