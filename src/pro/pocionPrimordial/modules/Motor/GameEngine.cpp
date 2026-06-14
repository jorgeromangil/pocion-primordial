#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameState.h"

class GameEngine {
public:
    GameEngine();
    void run();

    void changeState(std::shared_ptr<GameState> newState);
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window;
    std::shared_ptr<GameState> currentState;
};