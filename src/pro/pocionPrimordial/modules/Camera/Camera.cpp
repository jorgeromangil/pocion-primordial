#include "Camera.h"

Camera::Camera(sf::RenderWindow &window) : window(window), view(window.getDefaultView()) {}

void Camera::update(const sf::Vector2f &targetPosition)
{
    view.setCenter(targetPosition);
}

void Camera::apply()
{
    window.setView(view);
}