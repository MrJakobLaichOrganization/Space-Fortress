#pragma once

#include "graphics/tilemap.hpp"

class Resources
{
public:
    sf::Texture tilesTexture = sf::Texture::loadFromFile(ASSETS_DIR "/TileMap_SF/tileset_sf.png").value();
    TileSheet tileSheet{tilesTexture, {32, 32}};

    static Resources& get()
    {
        static Resources res;
        return res;
    }

private:
    Resources() = default;
};