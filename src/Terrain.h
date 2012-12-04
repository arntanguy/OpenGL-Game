#ifndef __Terrain__
#define __Terrain__

#include "TerrainPage.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class Terrain {
    private:
        std::vector<TerrainPage *> mTerrainPages;
        int mTileSize;
        int mScale;
        int mMaxHeight;

        void load();

    public:
        Terrain(int tileSize, int scale, int maxHeight);
        virtual ~Terrain();

        sf::Vector2i getCurrentTile(sf::Vector3f worldPos);

        bool render();
};

#endif
