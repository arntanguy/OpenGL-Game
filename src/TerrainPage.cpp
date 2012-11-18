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


    mMixmap = new sf::Image();
    mMixmap->create(width, depth, sf::Color(0,0,0));

    mTexture0.loadTexture("assets/crate.jpg");
    mTexture1.loadTexture("assets/l3d.jpg");
    mTexture2.loadTexture("assets/terrain/blue.jpg");
    mTexture3.loadTexture("assets/terrain/rock.png");
    // Load temporary texture
    mMixmapTexture.loadTexture(mMixmap, "Mixmap");

    texShader.loadVertexShader("assets/shaders/vertex/blend_mixmap_vertex.glsl");
    texShader.loadFragmentShader("assets/shaders/fragment/blend_textures_from_mixmap.glsl");
    texShader.setFloat("texscale", 100);
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
    return mMaxHeight*float(c.r + c.g + c.b)/(3.0f*255.f);
}

/**
 * Calculates all the vertices positions
 * Maps texture coordinates
 **/
void TerrainPage::generate()
{
    int vertI = 0;
    int indI = 0;
    sf::Vector2u texcoord;
    for(int z = mDepth; z > 1; z--) {
        for(int x = 0; x < mWidth-1; x++) {
            Vertex v1, v2, v3;

            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            if(((x)%mWidth) == 0) {
                v1.position[0] = float(x);
                v1.position[1] = getHeight(x, z);
                v1.position[2] = float(z);
                texcoord = getTextureCoordinates(x, z);
                v1.texcoords[0] = texcoord.x;
                v1.texcoords[1] = texcoord.y;
                mVertices.push_back(v1);
            }

            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            v2.position[0] = float(x);
            v2.position[1] = getHeight(x, z-1);
            v2.position[2] = float(z-1);
            texcoord = getTextureCoordinates(x, z-1);
            v2.texcoords[0] = texcoord.x;
            v2.texcoords[1] = texcoord.y;
            mVertices.push_back(v2);

            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            v3.position[0] = float(x+1);
            v3.position[1] = getHeight(x+1, z);
            v3.position[2] = float(z);
            texcoord = getTextureCoordinates(x+1, z);
            v3.texcoords[0] = texcoord.x;
            v3.texcoords[1] = texcoord.y;
            mVertices.push_back(v3);
        }
        // Get the color of the pixel at the (by ratio times grid position) location
        Vertex v;
        // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
        v.position[0] = float(mWidth-1);
        v.position[1] = getHeight(mWidth-1, z-1);
        v.position[2] = float(z-1);
        texcoord = getTextureCoordinates(mWidth-1, z-1);
        v.texcoords[0] = texcoord.x;
        v.texcoords[1] = texcoord.y;
        mVertices.push_back(v);

    }
    generateDisplayList();
}

/**
 * Compile the openGL commands needed to render the scene
 */
void TerrainPage::generateDisplayList()
{
    // create one display list
    mDisplayListIndex = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(mDisplayListIndex, GL_COMPILE);
    glFrontFace( GL_CW  ); //   Vertex are added clockwise. Used to calculate normals
    std::vector<Vertex>::iterator it;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    texShader.enable();
    texShader.bindTexture(mTexture0, "Texture0", 0);
    texShader.bindTexture(mTexture1, "Texture1", 1);
    texShader.bindTexture(mTexture2, "Texture2", 2);
    texShader.bindTexture(mTexture3, "Texture3", 3);
    texShader.bindTexture(mMixmapTexture, "Mixmap", 4);
    Vertex v;
    int j=0;
    glEnable(GL_TEXTURE_2D);
    //mTexture.bind();
    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i<mVertices.size(); i++) {
        if(i%(2*mWidth) == 0) glEnd(); glBegin(GL_TRIANGLE_STRIP);
        v = mVertices[i];
        glTexCoord2f(v.texcoords[0], v.texcoords[1]);
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
        std::cerr << "Error: The terrain hasn't been compiled! You need to call the generate() function first!" << std::endl;
        return false;
    } else {
        // draw the display list
        glCallList(mDisplayListIndex);
        return true;
    }
}
