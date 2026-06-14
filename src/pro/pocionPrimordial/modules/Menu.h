#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>

#define OPTION_HEIGHT 125

class Menu {
public:
    Menu(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void navigate(sf::Keyboard::Key key);
    int getSelectedOption() const;
    void showCredits(sf::RenderWindow &window);

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture controlsTexture;
    sf::Sprite controlsSprite;

    sf::Texture creditsTexture;
    sf::Sprite creditsSprite;

    sf::Font font;
    sf::Text text[3];
    std::string options[3];
    int selectedOption;

    void drawBackground(sf::RenderWindow &window);
    void drawControls(sf::RenderWindow &window);
    void drawCredits(sf::RenderWindow &window);
};

#endif
