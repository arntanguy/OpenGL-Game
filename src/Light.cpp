#include "Light.h"

Light::Light(GLuint lightId)
{
    mLightId = lightId;
}

Light::Light(GLuint lightId, const sf::Vector3f& ambiant, const sf::Vector3f& diffuse, const sf::Vector3f& specular) : mLightId(lightId)
{
    setAmbiant(ambiant);
    setDiffuse(diffuse);
    setSpecular(specular);
}

Light::~Light()
{
}


sf::Vector3f Light::getAmbiant() const
{
    return mAmbiant;
}
sf::Vector3f Light::getDiffuse() const
{
    return mDiffuse;
}
sf::Vector3f Light::getSpecular() const
{
    return mSpecular;
}

void Light::setAmbiant (const sf::Vector3f& value)
{
    GLfloat Al[4];
    Al[0] = value.x;
    Al[1] = value.y;
    Al[2] = value.z;
    Al[3] = 1;

    glLightfv( mLightId, GL_AMBIENT, Al );
    mAmbiant = value;
}
void Light::setDiffuse (const sf::Vector3f& value)
{
    mDiffuse = value;
    GLfloat Dl[4];
    Dl[0] = value.x;
    Dl[1] = value.y;
    Dl[2] = value.z;
    Dl[3] = 1;
    glLightfv( mLightId, GL_DIFFUSE, Dl );
}
void Light::setSpecular(const sf::Vector3f& value)
{
    mSpecular = value;
    GLfloat Sl[4];
    Sl[0] = value.x;
    Sl[1] = value.y;
    Sl[2] = value.z;
    Sl[3] = 1;
    glLightfv( mLightId, GL_SPECULAR, Sl );
}

/**
 * ========== VIRTUALS========
 */
bool Light::generate()
{
    return true;
}
bool Light::render()
{
    return true;
}
