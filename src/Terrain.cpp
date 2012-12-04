#include "Terrain.h"


Terrain::Terrain(int tileSize, int scale, int maxHeight)
{
    mScale = scale;
    mTileSize = tileSize;
    mMaxHeight = maxHeight;
    load();
}

Terrain::~Terrain()
{
}

void Terrain::load()
{
    std::string basePath = "assets/terrain/test_heightmap/heightmap-";
    for(int i=0; i<4; i++) {
        std::stringstream ss;
        ss << basePath << i << ".jpg";
        std::string path = ss.str();

        dinf << "Loading terrain page " << path << std::endl;

        TerrainPage *terrain = new TerrainPage(path, mTileSize, mMaxHeight, mScale);
        sf::Image *mixmap = terrain->getMixmap();
        for(int x = 0; x < mixmap->getSize().x; x++) {
            for(int y = 0; y < mixmap->getSize().y; y++) {
                //float height = terrain->getHeight(x, y);
                float height=0;
                if(height < 5)
                    mixmap->setPixel(x, y, sf::Color(255, 0, 0, 0));
                else if (height < 10)
                    mixmap->setPixel(x, y, sf::Color(50, 200, 0, 0));
                else if (height < 20)
                    mixmap->setPixel(x, y, sf::Color(0, 150, 150, 0));
                else if (height < 30)
                    mixmap->setPixel(x, y, sf::Color(0, 0, 150, 50));
                else
                    mixmap->setPixel(x, y, sf::Color(0, 0, 0, 255));
            }
        }
        std::stringstream ss2;
        ss2 << basePath << i << "-mixmap.jpg";
        std::string path2 = ss2.str();
        terrain->setMixmap(path2);
        terrain->generateVertices();
        mTerrainPages.push_back(terrain);
    }
}


sf::Vector2i Terrain::getCurrentTile(sf::Vector3f worldPos)
{
    return sf::Vector2i(worldPos.x/mTileSize, worldPos.y/mTileSize);
}

bool Terrain::render()
{
    std::vector<TerrainPage*>::const_iterator it = mTerrainPages.begin();
    int i=0;
    glPushMatrix();
        glPushMatrix();
            mTerrainPages[0]->render();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(mTileSize*mScale, 0, 0);
            mTerrainPages[1]->render();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(mTileSize*mScale, 0, mTileSize*mScale);
            mTerrainPages[3]->render();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, 0, mTileSize*mScale);
            mTerrainPages[2]->render();
        glPopMatrix();
    glPopMatrix();
    //for(; it != mTerrainPages.end(); it++) {
    //    glPushMatrix();
    //        (*it)->render();
    //    glPopMatrix();
    //    i++;
    //}
}
