#include "FlagEntity.h"

FlagEntity::FlagEntity(float width, float height, float nbSquares) : Entity()
{
    mWidth = width;
    mHeight = height;
    mNbSquares = nbSquares;
    mDisplayListIndex = OBJECT_NOT_COMPILED;
    //mTexture = texture;
}

FlagEntity::~FlagEntity()
{
    dinf << "GrassEntiy::~FlagEntity()" << std::endl;
}

bool FlagEntity::generate()
{
    // create one display list
    mDisplayListIndex = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(mDisplayListIndex, GL_COMPILE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_TEXTURE_2D);
        //mTexture.bind();



        glBegin(GL_TRIANGLES);
            glColor3f(1,0,0);
            float widthSquare=mWidth/mNbSquares;
            float mHeightSquare=mHeight;
            for(float i=0; i<mNbSquares; i++) {
                // First triangle
                //Bottom right
                glTexCoord2d(0,(i+1)/mNbSquares);
                glVertex3f((i+1)*widthSquare, 0.0, 0.0);
                // Top left
                glTexCoord2d(i/mNbSquares,1);
                glVertex3f(i*widthSquare, mHeightSquare, 0.0);
                // Bottom left
                glTexCoord2d(i/mNbSquares,i/mNbSquares);
                glVertex3f(i*widthSquare, 0.0, 0.0);


                // Second triangle
                // Bottom right
                glTexCoord2d(0,(i+1)/mNbSquares);
                glVertex3f((i+1)*widthSquare, 0.0, 0.0);
                //Top right
                glTexCoord2d((i+1)/mNbSquares,1);
                glVertex3f((i+1)*widthSquare, mHeightSquare, 0.0);
                // Top left
                glTexCoord2d(i/mNbSquares,1);
                glVertex3f(i*widthSquare, mHeightSquare, 0.0);
            }
        glEnd();

//        glDisable(GL_TEXTURE_2D);
    glEndList();
}

bool FlagEntity::render()
{
    if(mDisplayListIndex == OBJECT_NOT_COMPILED) {
        derr << "Error: You must first call the generate function to compile the opengl command list" << std::endl;
        return false;
    } else {
        // draw the display list
        glCallList(mDisplayListIndex);
        return true;
    }
}

float FlagEntity::getNbSquares() const
{
    return mNbSquares;
}

float FlagEntity::getWidth() const
{
    return mWidth;
}

float FlagEntity::getHeight() const
{
    return mHeight;
}
