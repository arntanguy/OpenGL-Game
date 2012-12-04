#ifndef __TerrainPage__
#define __TerrainPage__

#include "Shader.h"
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <list>

#include "Texture.h"
#include "Node.h"
#include "GrassEntity.h"
#include "Debug.h"


#define TERRAIN_NOT_COMPILED -1

struct Vertex;

/**
 * This class loads a terrain from a heightmap.
 * Requires: SFML
 **/
class TerrainPage
{
    private:
        sf::Image *mHeightmap;
        sf::Image *mMixmap;
        // Width and depth of the landscape
        int mSize;
        float mScaleFactor;
        /*
         * Width and depth ratio.
         * Used to convert from terrain coordinates to heightmap coordinates
         */
        int mRatioW, mRatioD;
        float mMaxHeight;

        //std::vector<Vertex> mVertices;
        Vertex *mVertices;

        GLuint mDisplayListIndex;
        void generateVerticesDisplayList();

        sf::Vector2u getTextureCoordinates(float x, float z);

        /**
         * 4 textures to be merged according to mixmap
         */
        Texture *mTexture0;
        Texture *mTexture1;
        Texture *mTexture2;
        Texture *mTexture3;
        Texture *mMixmapTexture;
        Texture *mHeightmapTexture;

        /**
         * Shader blending the textures according to the mixmap
         */
        Shader mTexShader;

        Node *mNode;
        Entity *grass;

        void loadTexture(const std::string& texturePath);

        sf::Clock mWaveClock;
        bool mWaveActivated;

    public:
        TerrainPage(const std::string& heightmap, int size, float mMaxHeight, float scaleFactor);
        virtual ~TerrainPage();

        /**
         * Computes the terrain
         */
        void generateVertices();

        bool render();
        float getHeight(int x, int z);
        float getHeightFromHeighmapCoordinates(int x, int y);

        sf::Image * getMixmap() const
        {
            return mMixmap;
        }
        void setMixmap(std::string name)
        {
            // XXX: debug only
            mMixmap->saveToFile(name);
            mMixmapTexture->loadTexture(mMixmap, name);
        }
        void startWave(bool status);
};


struct Vertex
{
	GLfloat position[3];
	GLfloat	texcoords[2];
	GLfloat normal[3];
};


#endif
