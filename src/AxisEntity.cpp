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
  /* Axis */
  glBegin(GL_LINES);
  //X axis
  glColor3f(mScale, 0, 0); glVertex3f(-mScale, -mScale, -mScale); glVertex3f( mScale, -mScale, -mScale);
  //Y axis
  glColor3f(0, mScale, 0); glVertex3f(-mScale, -mScale, -mScale); glVertex3f(-mScale,  mScale, -mScale);
  //Z axis
  glColor3f(0, 0, mScale); glVertex3f(-mScale, -mScale, -mScale); glVertex3f(-mScale, -mScale,  mScale);
  glEnd();

}
