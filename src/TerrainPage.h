#ifndef __TerrainPage__
#define __TerrainPage__

#include "Shader.h"
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>

#include "RessourcesManager.h"
#include "Texture.h"

#define TERRAIN_NOT_COMPILED -1

struct Vertex;

/**
 * This class loads a terrain from a heightmap.
 * Requires: SFML
 **/
class TerrainPage
{
    private:
        sf::Image mHeightmap;
        sf::Image *mMixmap;
        // Width and depth of the landscape
        int mWidth, mDepth;
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
        Texture mTexture0;
        Texture mTexture1;
        Texture mTexture2;
        Texture mTexture3;
        Texture mMixmapTexture;
        Shader texShader;

        void loadTexture(const std::string& texturePath);


    public:
        TerrainPage(const std::string& heightmap, int width, int depth, float mMaxHeight);
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
        void setMixmap()
        {
            mMixmap->saveToFile("MixmapInTerrain.jpg");
            mMixmapTexture.loadTexture(mMixmap, "Mixm");
        }
};


struct Vertex
{
	GLfloat position[3];
	GLfloat	texcoords[2];
	GLfloat normal[3];
};


#endif
