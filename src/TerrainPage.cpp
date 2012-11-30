#include "TerrainPage.h"
#include <GL/glut.h>
#include "tick.h"
#include "RessourcesManager.h"

TerrainPage::TerrainPage(const std::string& heightmap, int width, int depth, float maxHeight, float scaleFactor)
{
    mWidth = width;
    mDepth = depth;
    mScaleFactor = scaleFactor;
    mHeightmap = new sf::Image();
    mHeightmap->loadFromFile(heightmap);
    mRatioW = mHeightmap->getSize().x/width;
    mRatioD = mHeightmap->getSize().y/depth;
    mMaxHeight = maxHeight;
    mDisplayListIndex = TERRAIN_NOT_COMPILED;
    mWaveActivated = false;

    // *width-1 to account for the diagonals
    mVertices = new Vertex[(mWidth*mDepth)+(mWidth-1)*(mDepth-1)];
    dinf << "TerrainPage: Number of vertices: " <<(mWidth)*(mDepth)+(mWidth-1)*(mDepth-1)<< std::endl;

    mMixmap = new sf::Image();
    mMixmap->create(width, depth, sf::Color(0,0,0));

    mTexture0 = RessourcesManager::getInstance().loadTexture("assets/terrain/water2.png");
    mTexture1 = RessourcesManager::getInstance().loadTexture("assets/terrain/seamless_beach_sand_texture_by_hhh316-d4hr45u.jpg");
    mTexture2 = RessourcesManager::getInstance().loadTexture("assets/terrain/Seamless_grass_texture_by_hhh316.jpg");
    mTexture3 = RessourcesManager::getInstance().loadTexture("assets/terrain/seamless_rock.jpg");
    // Load temporary texture
    mMixmapTexture = new Texture();
    mMixmapTexture->loadTexture(mMixmap, "Mixmap");
    RessourcesManager::getInstance().addTexture(mMixmapTexture, "Mixmap");
    mHeightmapTexture = new Texture();
    mHeightmapTexture->loadTexture(mHeightmap, "Heightmap");
    RessourcesManager::getInstance().addTexture(mHeightmapTexture, "Heightmap");

    mTexShader.loadVertexShader("assets/shaders/vertex/blend_mixmap_vertex.glsl");
    mTexShader.loadFragmentShader("assets/shaders/fragment/blend_textures_from_mixmap.glsl");


    Texture *grassText = RessourcesManager::getInstance().loadTexture("assets/quick_grass.png");
    grass = new GrassEntity(grassText, "assets/shaders/vertex/default_vertex_renderer.glsl", "assets/shaders/fragment/render_one_texture.glsl");
    grass->generate();
    mNode = new Node("GrassField");
    mNode->attachEntity(grass);
}

TerrainPage::~TerrainPage()
{
    delete mVertices;
}

void TerrainPage::loadTexture(const std::string& texturePath)
{
//    mTexture.loadTexture(texturePath);
}

sf::Vector2u TerrainPage::getTextureCoordinates(float x, float z)
{
    return sf::Vector2u(int(x)%2, int(z)%2);
}

// Get the color of the pixel at the (by ratio times grid position) location
float TerrainPage::getHeight(int x, int z)
{
    // Get the color of the pixel at the (by ratio times grid position) location
    sf::Color c = mHeightmap->getPixel(x*mRatioW, z*mRatioD);
    return mMaxHeight*float(c.r + c.g + c.b)/(3.0f*255.f);
}

float TerrainPage::getHeightFromHeighmapCoordinates(int x, int y)
{
    // Get the color of the pixel at the (by ratio times grid position) location
    sf::Color c = mHeightmap->getPixel(x, y);
    return (float)(mMaxHeight*float(c.r + c.g + c.b)/(3.0f*255.f));
}

/**
 * Calculates all the vertices positions
 * Maps texture coordinates
 * XXX: Needs to compute normals too!!!!
 **/
void TerrainPage::generateVertices()
{
    /**
     * Variables used for the generate grid algorithm
     */
    bool add = true;
    int i = 0;
    int loop = 0;
    int stepByRow = 2*mWidth;
    int substractVal = mWidth-1;
    int j=0;
    int row=0;

    // Texture coordinates
    sf::Vector2u texCoords;

    /**
     * Loop to generateVertices all the vertices of the grid,
     * with a minimum of vertex!
     **/
    while(j < (stepByRow)*(mDepth-1)) {
        int x = i%mWidth;
        int z = mDepth-i/mWidth;
        Vertex v;
        v.position[0] = x*mScaleFactor;//(x-mWidth/2)*mScaleFactor;
        v.position[1] = getHeight(x,z);
        v.position[2] = z*mScaleFactor;//(z-mWidth/2)*mScaleFactor;
        texCoords = getTextureCoordinates(x, z);
        v.texcoords[0] = texCoords.x;
        v.texcoords[1] = texCoords.y;
        mVertices[j] = v;

        // Little glue to change row:
        // Loop current indice
        // Update substract value to get the right indices on next row
        if(loop == stepByRow-1) {
            mVertices[++j] = v;
            loop = 0;
            if(substractVal == mWidth-1) substractVal = mWidth+1;
            else substractVal = mWidth-1;
            add = !add;
            row++;
        }

        if(add) i += mWidth;
        else i -= substractVal;
        add = !add;
        loop++;
        j++;
    }

    generateVerticesDisplayList();
}

/**
 * Compile the openGL commands needed to render the scene
 */
void TerrainPage::generateVerticesDisplayList()
{
    // create one display list
    mDisplayListIndex = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(mDisplayListIndex, GL_COMPILE);
    glFrontFace( GL_CW  ); //   Vertex are added clockwise. Used to calculate normals

    Vertex v;
    int j=0;

    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i<(2*mWidth)*(mDepth-1); i++) {
        v = mVertices[i];
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, v.texcoords[0], v.texcoords[1]);
        glVertex3f(v.position[0], v.position[1], v.position[2]);
    }
    glEnd();

    glEndList();
}

void TerrainPage::startWave(bool status)
{
    mWaveActivated = status;
    mWaveClock.restart();
}

bool TerrainPage::render()
{
    // Used to modulate the water texture coordinates by the shader.
    static float add = 0.005;
    static float sinus = -1;

    if(mDisplayListIndex == TERRAIN_NOT_COMPILED) {
        std::cerr << "Error: The terrain hasn't been compiled! You need to call the generateVertices() function first!" << std::endl;
        return false;
    } else {
       mTexShader.enable();
       mTexShader.bindTexture(mTexture0, "Texture0");
       mTexShader.bindTexture(mTexture1, "Texture1");
       mTexShader.bindTexture(mTexture2, "Texture2");
       mTexShader.bindTexture(mTexture3, "Texture3");
       mTexShader.bindTexture(mMixmapTexture, "Mixmap");
       mMixmapTexture->getImage()->saveToFile("Mixmap.jpg");
       mTexShader.bindTexture(mHeightmapTexture, "Heightmap");
       mHeightmapTexture->getImage()->saveToFile("Heightmap.jpg");
       mTexShader.setFloat("terrainSize", mWidth*mScaleFactor);
       mTexShader.setFloat("maxHeight", mMaxHeight);
       mTexShader.setFloat("fogFactor", 0.);
       mTexShader.setFloat("waterSinus", sinus);
       mTexShader.setFloat("waterHeight", 5);
       mTexShader.setInt("nbLights", 2);

       if(mWaveActivated)
           mTexShader.setFloat("waveActivated", 1.);
       else
           mTexShader.setFloat("waveActivated", 0.);
       sf::Time time = mWaveClock.getElapsedTime();
       mTexShader.setFloat("waveTime", time.asSeconds()/10);

        if(sinus+add >= 1.f ) add = -0.005;
        if(sinus+add <= -1.f) add = 0.005;
        sinus+=add;

        // draw the display list
        glCallList(mDisplayListIndex);

    glEnable(GL_TEXTURE_2D);
        mNode->translate(300, getHeight(300,300), 300);
        mNode->scale(1);
        mNode->render();
    glDisable(GL_TEXTURE_2D);

        mTexShader.disable();
        return true;
    }
}
