#ifndef __AxisEntity__
#define __AxisEntity__

#include <GL/gl.h>
#include "Entity.h"

class AxisEntity : public Entity {
    private:
        int mScale;
    public:
        AxisEntity(int scale=1);
        virtual ~AxisEntity();

        bool generate();
        bool render();
};

#endif
