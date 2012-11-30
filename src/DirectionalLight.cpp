#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(GLuint lightId) : Light(lightId)
{
}

DirectionalLight::DirectionalLight(GLuint lightId, const sf::Vector3f& direction) : Light(lightId), mDirection(direction)
{
}

DirectionalLight::DirectionalLight(GLuint lightId, const sf::Vector3f& direction, const sf::Vector3f& ambiant, const sf::Vector3f& diffuse, const sf::Vector3f& specular) : Light(lightId, ambiant, diffuse, specular), mDirection(direction)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setDirection(const sf::Vector3f& direction)
{
    mDirection = direction;

    GLfloat d[4];
    d[0] = direction.x;
    d[1] = direction.y;
    d[2] = direction.z;
    d[3] = 0; // directional light

	glLightfv(mLightId, GL_POSITION, d);
}

sf::Vector3f DirectionalLight::getDirection() const
{
    return mDirection;
}
