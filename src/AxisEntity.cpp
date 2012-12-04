#include "AxisEntity.h"

AxisEntity::AxisEntity(int scale)
{
    mScale = scale;
}

AxisEntity::~AxisEntity()
{
}

bool AxisEntity::generate()
{
    return true;
}


bool AxisEntity::render()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    /* Axis */
    glBegin(GL_LINES);
    //X axis
    glColor3f(mScale, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f( mScale, 0, 0);
    //Y axis
    glColor3f(0, mScale, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0,  mScale, 0);
    //Z axis
    glColor3f(0, 0, mScale);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, mScale);

    glEnd();

    glPopAttrib();
}
