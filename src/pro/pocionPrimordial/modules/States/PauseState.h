#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class PauseState : public State
{
public:
    PauseState(sf::RenderWindow &window);

    void handleInput(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window, float alpha) override; // Cambiado para coincidir con la firma base

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Music menuMusic;
    sf::SoundBuffer optionSoundBuffer;
    sf::Sound optionSound;
    sf::Font font;
    sf::Text text[2];
    std::string options[2];
    int selectedOption;

    const int OPTION_HEIGHT = 125; // Altura entre las opciones del menú

    void navigate(sf::Keyboard::Key key);
    void updateOptionPositions();
};

#endif // PAUSESTATE_H
