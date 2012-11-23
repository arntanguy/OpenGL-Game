#include "GrassEntity.h"

GrassEntity::GrassEntity(Texture* texture) : Entity()
{
    mDisplayListIndex = OBJECT_NOT_COMPILED;
    mTexture = texture;
}

GrassEntity::~GrassEntity()
{
    dinf << "GrassEntiy::~GrassEntity()" << std::endl;
}

bool GrassEntity::generate()
{
    // create one display list
    mDisplayListIndex = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(mDisplayListIndex, GL_COMPILE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        mTexture->bind();



        glBegin(GL_TRIANGLES);
            glTexCoord2d(0,1);
            glVertex3f(5.0, 0.0, 0.0);
            glTexCoord2d(1,0);
            glVertex3f(-5.0, 10.0, 0.0);
            glTexCoord2d(1,1);
            glVertex3f(-5.0, 0.0, 0.0);

            glTexCoord2d(0,1);
            glVertex3f(5.0, 0.0, 0.0);
            glTexCoord2d(0,0);
            glVertex3f(5.0, 10.0, 0.0);
            glTexCoord2d(1,0);
            glVertex3f(-5.0, 10.0, 0.0);

            //third triangle
            glTexCoord2d(0,1);
            glVertex3f(2.5, 0.0, 4.3);
            glTexCoord2d(1,0);
            glVertex3f(-2.5, 10.0, -4.3);
            glTexCoord2d(1,1);
            glVertex3f(-2.0, 0.0, -4.3);
            //fourth triangle
            glTexCoord2d(0,1);
            glVertex3f(2.5, 0.0, 4.3);
            glTexCoord2d(0,0);
            glVertex3f(2.5, 10.0, 4.3);
            glTexCoord2d(1,0);
            glVertex3f(-2.5, 10.0, -4.3);

            ////fifth triangle
            glTexCoord2d(0,1);
            glVertex3f(2.5, 0.0, -4.3);
            glTexCoord2d(1,0);
            glVertex3f(-2.5, 10.0, 4.3);
            glTexCoord2d(1,1);
            glVertex3f(-2.0, 0.0, 4.3);
            //sixth triangle
            glTexCoord2d(0,1);
            glVertex3f(2.5, 0.0, -4.3);
            glTexCoord2d(0,0);
            glVertex3f(2.5, 10.0, -4.3);
            glTexCoord2d(1,0);
            glVertex3f(-2.5, 10.0, 4.3);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    glEndList();
}

bool GrassEntity::render()
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
