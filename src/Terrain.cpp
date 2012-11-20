#include "Terrain.h"


Terrain::Terrain(int tileSize, int scale, int nbTiles, int lodFalloff)
{
    mScale = scale;
    mNbTiles = nbTiles;
    mTileSize = tileSize;
    mLodFalloff = lodFalloff;
}

Terrain::~Terrain()
{
}

void Terrain::loadTile(const sf::Vector2i& tile)
{
    //TerrainPage *page = new TerrainPage("assets/terrain/heightmap.bmp", mTileSize);
}

sf::Vector2i Terrain::getCurrentTile(sf::Vector3f worldPos)
{
    return sf::Vector2i(worldPos.x/mTileSize, worldPos.y/mTileSize);
}
