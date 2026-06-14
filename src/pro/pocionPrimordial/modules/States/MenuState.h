#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include "GameManager.h"
#include "Menu.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> // Añadido para sf::Texture y sf::Sprite

class MenuState : public State
{
public:
    MenuState(sf::RenderWindow &window);
    virtual void handleInput(sf::RenderWindow &window) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow &window, float alpha) override;

private:
    Menu menu;
    sf::Music menuMusic;
    sf::SoundBuffer optionSoundBuffer;
    sf::Sound optionSound;
    sf::Texture tutorialTexture;
    sf::Sprite tutorialSprite;
    bool showingTutorial;
};

#endif