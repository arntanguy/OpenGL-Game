#include "FlagEntity.h"

FlagEntity::FlagEntity(float width, float height, float nbSquares) : Entity()
{
    mWidth = width;
    mHeight = height;
    mNbSquares = nbSquares;
    mDisplayListIndex = OBJECT_NOT_COMPILED;
    mTexture = 0;
}

FlagEntity::FlagEntity(Texture* texture, float width, float height, float nbSquares) : Entity()
{
    mWidth = width;
    mHeight = height;
    mNbSquares = nbSquares;
    mDisplayListIndex = OBJECT_NOT_COMPILED;
    mTexture = texture;
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

        if(mTexture != 0) {
            glEnable(GL_TEXTURE_2D);
            mTexture->bind();
        } else {
            glColor3f(1,0,0);
        }


        glBegin(GL_TRIANGLES);
            float widthSquare=mWidth/mNbSquares;
            float mHeightSquare=mHeight;
            for(float i=0; i<mNbSquares; i++) {
                // First triangle
                //Bottom right
                glTexCoord2d((i+1)/mNbSquares,1);
                glVertex3f((i+1)*widthSquare, 0.0, 0.0);
                // Top left
                glTexCoord2d(i/mNbSquares,0);
                glVertex3f(i*widthSquare, mHeightSquare, 0.0);
                // Bottom left
                glTexCoord2d(i/mNbSquares,1);
                glVertex3f(i*widthSquare, 0.0, 0.0);


                // Second triangle
                // Bottom right
                glTexCoord2d((i+1)/mNbSquares,1);
                glVertex3f((i+1)*widthSquare, 0.0, 0.0);
                //Top right
                glTexCoord2d((i+1)/mNbSquares,0);
                glVertex3f((i+1)*widthSquare, mHeightSquare, 0.0);
                // Top left
                glTexCoord2d(i/mNbSquares,0);
                glVertex3f(i*widthSquare, mHeightSquare, 0.0);
            }
        glEnd();

        if(mTexture != 0)
        glDisable(GL_TEXTURE_2D);
    glEndList();
}

bool FlagEntity::render()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    if(mDisplayListIndex == OBJECT_NOT_COMPILED) {
        derr << "Error: You must first call the generate function to compile the opengl command list" << std::endl;
        return false;
    } else {
        // draw the display list
        glCallList(mDisplayListIndex);
        return true;
    }
    glPopAttrib();
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
