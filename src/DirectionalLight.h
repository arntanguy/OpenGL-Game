#ifndef __DirectionalLight__
#define __DirectionalLight__

#include "Light.h"

class DirectionalLight : public Light {
    protected:
        sf::Vector3f mDirection;

    public:
        DirectionalLight(GLuint lightId);
        DirectionalLight(GLuint lightId, const sf::Vector3f& direction);
        DirectionalLight(GLuint lightId, const sf::Vector3f& direction, const sf::Vector3f& ambiant, const sf::Vector3f& diffuse, const sf::Vector3f& specular);
        ~DirectionalLight();

        virtual void setDirection(const sf::Vector3f& direction);
        sf::Vector3f getDirection() const;
};

#endif
