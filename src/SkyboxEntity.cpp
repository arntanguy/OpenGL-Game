#include "SkyboxEntity.h"


SkyboxEntity::SkyboxEntity(const std::string& texture, float halfSide) : Entity()
{
    mHalfSide = halfSide;
    mTexture = new Texture(texture);
    RessourcesManager::getInstance().addTexture(mTexture, texture);
}

SkyboxEntity::~SkyboxEntity()
{
}

bool SkyboxEntity::generate()
{
    return true;
}

bool SkyboxEntity::render()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // Do not write in depth buffer (infinite distance planes)
    glDepthMask(GL_FALSE);


    // assume we're working with the modelview
    glPushMatrix();
    glLoadIdentity();
    //glRotatef( 50, 0, 1, 0);
    //glRotatef( camera_pitch, 1.0f, 0.0f, 0.0f );
    //glRotatef( camera_yaw, 0.0f, 1.0f, 0.0f );

    /**
     * Draw Skybox
     **/

    glEnable(GL_TEXTURE_2D);
    mTexture->bind();
    // Render the left quad
    glBegin(GL_QUADS);
    //glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
    //glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    //glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    //glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 2./3.); glVertex3f(  mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(1./3., 2./3.); glVertex3f(  mHalfSide, -mHalfSide, -mHalfSide );
    glTexCoord2f(1./3., 1./3.); glVertex3f(  mHalfSide,  mHalfSide, -mHalfSide );
    glTexCoord2f(0, 1./3.); glVertex3f(  mHalfSide,  mHalfSide,  mHalfSide );
    glEnd();
    //
    // Render the front quad
    glBegin(GL_QUADS);
    glTexCoord2f(1./4., 2./3.); glVertex3f(  mHalfSide, -mHalfSide, -mHalfSide );
    glTexCoord2f(2./4., 2./3.); glVertex3f( -mHalfSide, -mHalfSide, -mHalfSide );
    glTexCoord2f(2./4., 1./3.); glVertex3f( -mHalfSide,  mHalfSide, -mHalfSide );
    glTexCoord2f(1./4., 1./3.); glVertex3f(  mHalfSide,  mHalfSide, -mHalfSide );
    glEnd();

    // Render the right quad
    glBegin(GL_QUADS);
    glTexCoord2f(2./4., 2./3.); glVertex3f( -mHalfSide, -mHalfSide, -mHalfSide );
    glTexCoord2f(3./4., 2./3.); glVertex3f( -mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(3./4., 1./3.); glVertex3f( -mHalfSide,  mHalfSide,  mHalfSide );
    glTexCoord2f(2./4., 1./3.); glVertex3f( -mHalfSide,  mHalfSide, -mHalfSide );
    glEnd();


    // Render the back quad
    glBegin(GL_QUADS);
    glTexCoord2f(3./4., 2./3.); glVertex3f( -mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(1, 2./3.); glVertex3f(  mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(1, 1./3.); glVertex3f(  mHalfSide,  mHalfSide,  mHalfSide );
    glTexCoord2f(3./4., 1./3.); glVertex3f( -mHalfSide,  mHalfSide,  mHalfSide );
    glEnd();


    // Render the top quad
    glBegin(GL_QUADS);
    glTexCoord2f(2./4., 1./3.); glVertex3f( -mHalfSide,  mHalfSide, -mHalfSide );
    glTexCoord2f(2./4., 0); glVertex3f( -mHalfSide,  mHalfSide,  mHalfSide );
    glTexCoord2f(1./4., 0); glVertex3f(  mHalfSide,  mHalfSide,  mHalfSide );
    glTexCoord2f(1./4., 1./3.); glVertex3f(  mHalfSide,  mHalfSide, -mHalfSide );
    glEnd();

    // Render the bottom quad
    glBegin(GL_QUADS);
    glTexCoord2f(1./4., 2./3.); glVertex3f( -mHalfSide, -mHalfSide, -mHalfSide );
    glTexCoord2f(1./4., 1); glVertex3f( -mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(2./4., 1); glVertex3f(  mHalfSide, -mHalfSide,  mHalfSide );
    glTexCoord2f(2./4., 2./3.); glVertex3f(  mHalfSide, -mHalfSide, -mHalfSide );
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glDepthMask(true);

    glPopAttrib();
    return true;
}
