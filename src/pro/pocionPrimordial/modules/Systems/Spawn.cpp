#pragma once
#include <vector>
#include "../Entities/Enemy.cpp"

class SpawnSystem {
public:
    void update(float dt);
    void render(sf::RenderWindow& window);
private:
    std::vector<Enemy> enemies;
    float spawnTimer = 1.f;
    float spawnInterval = 1.f;
};
