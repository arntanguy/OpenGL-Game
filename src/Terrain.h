#ifndef __Terrain__
#define __Terrain__

#include "TerrainPage.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class Terrain {
    private:
        std::vector<TerrainPage *> mTerrainPages;
        int mNbTiles;

    public:
        Terrain(int nbTiles);
        virtual ~Terrain();

        sf::Vector2i getCurrentTile(sf::Vector3f worldPos);
};

#endif
