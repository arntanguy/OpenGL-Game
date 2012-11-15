#ifndef __TerrainPage__
#define __TerrainPage__

#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>

struct Vertex;

/**
 * This class loads a terrain from a heightmap.
 * Requires: SFML
 **/
class TerrainPage
{
    private:
        sf::Image mHeightmap;
        // Width and depth of the landscape
        int mWidth, mDepth;
        /*
         * Width and depth ratio.
         * Used to convert from terrain coordinates to heightmap coordinates
         */
        int mRatioW, mRatioD;

        std::vector<Vertex> mVertices;
        std::vector<GLuint> mIndices;

        GLuint mDisplayListIndex;
        /**
         * Computes the terrain
         */
        void generate();
        void generateDisplayList();

    public:
        TerrainPage(const std::string& heightmap, int width, int depth);
        virtual ~TerrainPage();

        bool render();
};


struct Vertex
{
	GLfloat position[3];
	GLfloat	texcoords[2];
	GLfloat normal[3];
};


#endif
