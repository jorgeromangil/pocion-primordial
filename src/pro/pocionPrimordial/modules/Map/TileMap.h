#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

struct Layer {
    sf::VertexArray vertices;
    bool visible = true;
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string& tileset, const std::string& tmxFile);
    void setLayerVisible(const std::string& layerName, bool visible);
    bool isWalkable(int x, int y) const;

    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    int getTileWidth() const { return tileWidth; }
    int getTileHeight() const { return tileHeight; }
    sf::Vector2f getRandomWalkablePosition() const;
    
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture m_tileset;
    std::map<std::string, Layer> m_namedLayers;
    std::vector<std::string> m_layerOrder;

    int mapWidth = 0, mapHeight = 0;
    int tileWidth = 0, tileHeight = 0;

    std::vector<std::vector<bool>> walkableMap;

    // 🔍 Rectángulos para mostrar visualmente las colisiones (pared + objetos)
    std::vector<sf::RectangleShape> debugCollisionBoxes;
};

#endif // TILEMAP_H
