#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

class State
{
public:
    virtual void handleInput(sf::RenderWindow &window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window, float alpha) = 0;
    virtual ~State() = default; // Destructor virtual
};

#endif
