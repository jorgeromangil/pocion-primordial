#pragma once
#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(sf::RenderWindow &window);
    void update(const sf::Vector2f &targetPosition);
    void apply();

private:
    sf::RenderWindow &window;
    sf::View view;
};