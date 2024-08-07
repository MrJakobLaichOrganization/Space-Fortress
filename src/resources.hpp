#pragma once

class Resources
{
public:
    sf::Texture tilesTexture = sf::Texture::loadFromFile(ASSETS_DIR "/TileMap_SF/tileset_sf.png").value();

    static Resources& get()
    {
        static Resources res;
        return res;
    }

private:
    Resources() = default;
};