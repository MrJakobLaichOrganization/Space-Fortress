#pragma once

#include "graphics/tilemap.hpp"

#include <SFML/Graphics/Font.hpp>

class Resources
{
public:
    sf::Texture tilesTexture = sf::Texture(ASSETS_DIR "/TileMap_SF/tileset_sf.png");
    TileSheet tileSheet{tilesTexture, {32, 32}};

    sf::Font imbPlexMono = sf::Font(ASSETS_DIR "/IBMPlexMono-Regular.ttf");

    static Resources& get()
    {
        static Resources res;
        return res;
    }

private:
    Resources() = default;
};