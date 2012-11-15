#include "TerrainPage.h"
#include <GL/glut.h>

TerrainPage::TerrainPage(const std::string& heightmap, int width, int depth)
{
    mWidth = width;
    mDepth = depth;
    mHeightmap.loadFromFile(heightmap);
    mRatioW = mHeightmap.getSize().x/width;
    mRatioD = mHeightmap.getSize().y/depth;
    generate();
}

TerrainPage::~TerrainPage()
{
}

void TerrainPage::generate()
{
    std::cout << "Generate" << std::endl;
    int vertI = 0;
    int indI = 0;
    sf::Color c;
    for(int z = mDepth; z > 1; z--) {
        for(int x = 0; x < mWidth-1; x++) {
            Vertex v1, v2, v3;

            // Get the color of the pixel at the (by ratio times grid position) location
            c = mHeightmap.getPixel(x*mRatioW, z*mRatioD);

            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            if(((x)%mWidth) == 0) {
                v1.position[0] = float(x);
                v1.position[1] = float(c.r + c.g + c.b)/3.0f/15.0f;
                v1.position[2] = float(z);
                mVertices.push_back(v1);
            }

            // Get the color of the pixel at the (by ratio times grid position) location
            c = mHeightmap.getPixel(x*mRatioW, (z-1)*mRatioD);
            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            v2.position[0] = float(x);
            v2.position[1] = float(c.r + c.g + c.b)/3.0f/15.0f;
            v2.position[2] = float(z-1);
            mVertices.push_back(v2);

            // Get the color of the pixel at the (by ratio times grid position) location
            c = mHeightmap.getPixel((x+1)*mRatioW, z*mRatioD);
            // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
            v3.position[0] = float(x+1);
            v3.position[1] = float(c.r + c.g + c.b)/3.0f/15.0f;
            v3.position[2] = float(z);
            mVertices.push_back(v3);
        }
        // Get the color of the pixel at the (by ratio times grid position) location
        c = mHeightmap.getPixel(mWidth*mRatioW, z*mRatioD);
        Vertex v;
        // These are the grounds vertex-positions. Note that the y-component is the only one affected by the image
        v.position[0] = float(mWidth-1);
        v.position[1] = float(c.r + c.g + c.b)/3.0f/15.0f;
        v.position[2] = float(z-1);
        mVertices.push_back(v);

    }

    generateDisplayList();

    std::cout << "Generated" << std::endl;
}

void TerrainPage::generateDisplayList()
{
    // create one display list
    mDisplayListIndex = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(mDisplayListIndex, GL_COMPILE);
        std::vector<Vertex>::iterator it;

        Vertex v;
        int j=0;
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i<mVertices.size(); i++) {
            if(i%(2*mWidth) == 0) glEnd(); glBegin(GL_TRIANGLE_STRIP);
            glColor3f(1, 1, 1);
            v = mVertices[i];
            glVertex3f(v.position[0], v.position[1], v.position[2]);
            //std::cout <<v.position[0] << "\t" << v.position[1] <<"\t" <<  v.position[2] << "\t" << ++j << std::endl;
        }
        glEnd();
        //glBegin(GL_TRIANGLE_STRIP);
        //j=0;
        //for(int i = 2*mWidth; i<4*mWidth-2; i++) {
        //    glColor3f(1, 1, 1);
        //    v = mVertices[i];
        //    glVertex3f(v.position[0], v.position[1], v.position[2]);
        //    std::cout <<v.position[0] << "\t" << v.position[1] <<"\t" <<  v.position[2] << "\t" << ++j << std::endl;
        //}
        //glEnd();
    glEndList();
}

bool TerrainPage::render()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // draw the display list
    glCallList(mDisplayListIndex);
    return true;
}
