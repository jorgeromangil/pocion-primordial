#ifndef WINSTATE_H
#define WINSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

class WinState : public State
{
public:
    WinState(sf::RenderWindow &window);
    void handleInput(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window, float alpha) override;

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font font;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text returnText;

    void updatePositions(sf::RenderWindow &window);
};

#endif
