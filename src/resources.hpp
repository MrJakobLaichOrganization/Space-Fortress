#pragma once

#include "graphics/tilemap.hpp"

#include <SFML/Graphics/Font.hpp>

class Resources
{
public:
    sf::Texture tilesTexture = sf::Texture::loadFromFile(ASSETS_DIR "/TileMap_SF/tileset_sf.png").value();
    TileSheet tileSheet{tilesTexture, {32, 32}};

    sf::Font imbPlexMono = sf::Font::openFromFile(ASSETS_DIR "/IBMPlexMono-Regular.ttf").value();

    static Resources& get()
    {
        static Resources res;
        return res;
    }

private:
    Resources() = default;
};