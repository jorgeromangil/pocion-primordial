#include "Enemy.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f position) : attackCooldown(2.0f) {
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}
//sf::FloatRect getHitbox() const; 