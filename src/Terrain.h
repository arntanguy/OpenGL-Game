#ifndef __Terrain__
#define __Terrain__

#include "TerrainPage.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class Terrain {
    private:
        std::vector<TerrainPage *> mTerrainPages;
        int mNbTiles;
        int mLodFalloff;
        int mTileSize;
        int mScale;

    public:
        Terrain(int tileSize, int scale, int nbTiles, int lodFalloff);
        virtual ~Terrain();

        sf::Vector2i getCurrentTile(sf::Vector3f worldPos);
        void loadTile(const sf::Vector2i& tile);
};

#endif
