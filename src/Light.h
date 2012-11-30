/******************************************************************************
     Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
*                                                                             *
* This program is free software; you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation; either version 2 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc.,     *
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 ******************************************************************************/


#ifndef __Light__
#define __Light__

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include "Entity.h"

class Light : public Entity
{
    protected:
        GLuint mLightId;
        sf::Vector3f mAmbiant;
        sf::Vector3f mDiffuse;
        sf::Vector3f mSpecular;

    public:
        Light(GLuint lightId);
        Light(GLuint lightId, const sf::Vector3f& ambiant, const sf::Vector3f& diffuse, const sf::Vector3f& specular);
        ~Light();

        sf::Vector3f getAmbiant() const;
        sf::Vector3f getDiffuse() const;
        sf::Vector3f getSpecular() const;
        void setAmbiant (const sf::Vector3f& value);
        void setDiffuse (const sf::Vector3f& value);
        void setSpecular(const sf::Vector3f& value);

        // Virtuals
        virtual bool generate();
        virtual bool render();

};


#endif
