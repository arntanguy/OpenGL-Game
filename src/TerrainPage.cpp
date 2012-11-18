#include "TerrainPage.h"

TerrainPage::TerrainPage(const std::string& heightmap, int width, int depth, float maxHeight)
{
    mWidth = width;
    mDepth = depth;
    mHeightmap.loadFromFile(heightmap);
    mRatioW = mHeightmap.getSize().x/width;
    mRatioD = mHeightmap.getSize().y/depth;
    mMaxHeight = maxHeight;
    mDisplayListIndex = TERRAIN_NOT_COMPILED;

    // *width-1 to account for the diagonals
    mVertices = new Vertex[(mWidth*mDepth)+(mWidth-1)*(mDepth-1)];
    std::cout << "Terrain: Number of vertices: " <<(mWidth)*(mDepth)+(mWidth-1)*(mDepth-1)<< std::endl;

    mMixmap = new sf::Image();
    mMixmap->create(width, depth, sf::Color(0,0,0));

    mTexture0.loadTexture("assets/terrain/water.jpg");
    mTexture1.loadTexture("assets/terrain/sand.jpg");
    mTexture2.loadTexture("assets/terrain/grass.bmp");
    mTexture3.loadTexture("assets/terrain/rock.png");
    // Load temporary texture
    mMixmapTexture.loadTexture(mMixmap, "Mixmap");

    texShader.loadVertexShader("assets/shaders/vertex/blend_mixmap_vertex.glsl");
    texShader.loadFragmentShader("assets/shaders/fragment/blend_textures_from_mixmap.glsl");
}

TerrainPage::~TerrainPage()
{
}

void TerrainPage::loadTexture(const std::string& texturePath)
{
//    mTexture.loadTexture(texturePath);
}

sf::Vector2u TerrainPage::getTextureCoordinates(float x, float z) {
    return sf::Vector2u(int(x)%2, int(z)%2);
}

// Get the color of the pixel at the (by ratio times grid position) location
float TerrainPage::getHeight(int x, int z)
{
    // Get the color of the pixel at the (by ratio times grid position) location
    sf::Color c = mHeightmap.getPixel(x*mRatioW, z*mRatioD);
    return mMaxHeight*float(c.r + c.g + c.b)/(3.0f*255.f);
}

float TerrainPage::getHeightFromHeighmapCoordinates(int x, int y)
{
    // Get the color of the pixel at the (by ratio times grid position) location
    sf::Color c = mHeightmap.getPixel(x, y);
    return (float)(mMaxHeight*float(c.r + c.g + c.b)/(3.0f*255.f));
}

/**
 * Calculates all the vertices positions
 * Maps texture coordinates
 **/
void TerrainPage::generateVertices()
{
    bool add = true;
    int i = 0;
    int loop = 0;
    int stepByRow = 2*mWidth;
    int substractVal = mWidth-1;
    int j=0;
    int row=0;

    /**
     * Loop to generateVertices all the vertices of the grid,
     * with a minimum of vertex!
     **/
    while(j < (stepByRow)*(mDepth-1)) {
        int x = i%mWidth;
        int z = mDepth-i/mWidth;
        Vertex v;
        v.position[0] = x;
        v.position[1] = getHeight(x,z);
        v.position[2] = z;
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
    std::vector<Vertex>::iterator it;
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    texShader.enable();
    texShader.bindTexture(mTexture0, "Texture0", 0);
    texShader.bindTexture(mTexture1, "Texture1", 1);
    texShader.bindTexture(mTexture2, "Texture2", 2);
    texShader.bindTexture(mTexture3, "Texture3", 3);
    texShader.bindTexture(mMixmapTexture, "Mixmap", 4);
    texShader.setFloat("mixmapWidth", mMixmap->getSize().x);
    texShader.setFloat("mixmapHeight", mMixmap->getSize().y);
    Vertex v;
    int j=0;
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i<(2*mWidth)*(mDepth-1); i++) {
        v = mVertices[i];
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, v.texcoords[0], v.texcoords[1]);
        glVertex3f(v.position[0], v.position[1], v.position[2]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    texShader.disable();
    glEndList();
}


bool TerrainPage::render()
{
    if(mDisplayListIndex == TERRAIN_NOT_COMPILED) {
        std::cerr << "Error: The terrain hasn't been compiled! You need to call the generateVertices() function first!" << std::endl;
        return false;
    } else {
        // draw the display list
        glCallList(mDisplayListIndex);
        return true;
    }
}
