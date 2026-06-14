#include "TileMap.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>

using namespace tinyxml2;

bool TileMap::load(const std::string &tileset, const std::string &tmxFile)
{
    XMLDocument doc;
    if (doc.LoadFile(tmxFile.c_str()) != XML_SUCCESS)
    {
        std::cerr << "Error cargando el archivo TMX: " << tmxFile << std::endl;
        return false;
    }

    XMLElement *map = doc.FirstChildElement("map");
    map->QueryIntAttribute("width", &mapWidth);
    map->QueryIntAttribute("height", &mapHeight);
    map->QueryIntAttribute("tilewidth", &tileWidth);
    map->QueryIntAttribute("tileheight", &tileHeight);

    if (!m_tileset.loadFromFile(tileset))
    {
        std::cerr << "Error cargando tileset: " << tileset << std::endl;
        return false;
    }

    debugCollisionBoxes.clear();

    for (XMLElement *layer = map->FirstChildElement("layer"); layer != nullptr; layer = layer->NextSiblingElement("layer"))
    {
        std::string layerName = "sin_nombre";
        const char *name = layer->Attribute("name");
        if (name)
            layerName = name;

        m_layerOrder.push_back(layerName);

        sf::VertexArray vertices(sf::Quads);
        XMLElement *data = layer->FirstChildElement("data");

        if (!data)
        {
            std::cerr << "Capa sin datos: " << layerName << std::endl;
            continue;
        }

        if (layerName == "suelo")
        {
            walkableMap.resize(mapHeight, std::vector<bool>(mapWidth, false));
        }

        int x = 0, y = 0;
        for (XMLElement *tile = data->FirstChildElement("tile"); tile; tile = tile->NextSiblingElement("tile"))
        {
            if (!tile->Attribute("gid"))
            {
                ++x;
                if (x >= mapWidth)
                {
                    x = 0;
                    ++y;
                }
                continue;
            }

            int gid = tile->IntAttribute("gid");

            if (layerName == "suelo")
            {
                walkableMap[y][x] = (gid > 0);
            }

            if ((layerName == "pared" || layerName == "objetos") && gid > 0)
            {
                walkableMap[y][x] = false;
            }

            if (gid > 0)
            {
                gid -= 1;
                int columns = m_tileset.getSize().x / tileWidth;
                int tu = gid % columns;
                int tv = gid / columns;

                sf::Vertex v0, v1, v2, v3;

                v0.position = sf::Vector2f(x * tileWidth, y * tileHeight);
                v1.position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
                v2.position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
                v3.position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

                v0.texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                v1.texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                v2.texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                v3.texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                vertices.append(v0);
                vertices.append(v1);
                vertices.append(v2);
                vertices.append(v3);
            }

            ++x;
            if (x >= mapWidth)
            {
                x = 0;
                ++y;
            }
        }

        Layer l;
        l.vertices = vertices;
        l.visible = true;
        m_namedLayers[layerName] = l;
    }

    // Depuración: Contar tiles caminables
    int walkableCount = 0;
    for (const auto &row : walkableMap)
    {
        for (bool walkable : row)
        {
            if (walkable)
                ++walkableCount;
        }
    }

    if (walkableCount == 0)
    {
        std::cerr << "¡Error: No hay tiles caminables en el mapa!" << std::endl;
    }

    return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;

    for (const auto &layerName : m_layerOrder)
    {
        auto it = m_namedLayers.find(layerName);
        if (it != m_namedLayers.end() && it->second.visible)
        {
            target.draw(it->second.vertices, states);
        }
    }

    for (const auto &box : debugCollisionBoxes)
    {
        target.draw(box, states);
    }
}

void TileMap::setLayerVisible(const std::string &layerName, bool visible)
{
    auto it = m_namedLayers.find(layerName);
    if (it != m_namedLayers.end())
    {
        it->second.visible = visible;
    }
}

bool TileMap::isWalkable(int x, int y) const
{
    if (x < 0 || y < 0 || y >= (int)walkableMap.size() || x >= (int)walkableMap[0].size())
    {
        return false;
    }
    return walkableMap[y][x];
}

sf::Vector2f TileMap::getRandomWalkablePosition() const
{
    sf::Vector2f randomPosition;
    int attempts = 0;
    const int maxAttempts = 200; // Aumentado para mapas con pocos tiles caminables

    float mapWidthPx = mapWidth * tileWidth;
    float mapHeightPx = mapHeight * tileHeight;

    do
    {
        int tileX = rand() % mapWidth;
        int tileY = rand() % mapHeight;

        randomPosition.x = tileX * tileWidth + tileWidth / 2.f;
        randomPosition.y = tileY * tileHeight + tileHeight / 2.f;

        // Asegurar que la posición esté dentro de los límites
        if (randomPosition.x < 16.f || randomPosition.x > mapWidthPx - 16.f ||
            randomPosition.y < 16.f || randomPosition.y > mapHeightPx - 16.f)
        {
            ++attempts;
            continue;
        }

        ++attempts;
        if (attempts >= maxAttempts)
        {

            randomPosition.x = mapWidthPx / 2.f;
            randomPosition.y = mapHeightPx / 2.f;

            return randomPosition;
        }
    } while (!isWalkable(static_cast<int>(randomPosition.x / tileWidth), static_cast<int>(randomPosition.y / tileHeight)));

    return randomPosition;
}