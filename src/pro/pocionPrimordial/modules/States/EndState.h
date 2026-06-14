#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

class EndState : public State
{
public:
    EndState(sf::RenderWindow &window, int currentLevel); // Acepta el parámetro float
    void handleInput(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window, float alpha) override;

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    int currentLevel;

    sf::Font font;
    sf::Text titleText;
    sf::Text subtitleText;

    sf::Text nextLevelText;

    void updatePositions(sf::RenderWindow &window);
};

#endif // ENDSTATE_H
