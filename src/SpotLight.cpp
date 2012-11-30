#include "SpotLight.h"
#include <iostream>
#include "Debug.h"

SpotLight::SpotLight(GLuint lightId) : DirectionalLight(lightId)
{
    // No attenuation with distance
    setConstantAttenuation(1.0f);
    setLinearAttenuation(0.0f);
    setQuadricAttenuation(0.0f);
    setExponent(2.0f);
}

SpotLight::~SpotLight()
{
}



void SpotLight::setCutoff(float cutOff)
{
     //angle of the cone light emitted by the spot : value between 0 to 180
     glLightf(mLightId, GL_SPOT_CUTOFF, cutOff);
     mCutoff = cutOff;
}
float SpotLight::getCutoff() const
{
    return mCutoff;
}

void SpotLight::setExponent(float exponent)
{
     //exponent propertie defines the concentration of the light
     glLightf(mLightId, GL_SPOT_EXPONENT, 15.0f);
     mExponent = exponent;
}
float SpotLight::getExponent() const
{
    return mExponent;
}

void SpotLight::setConstantAttenuation(float constantAttenuation)
{
     glLightf(mLightId, GL_CONSTANT_ATTENUATION, constantAttenuation);
     mConstantAttenuation = constantAttenuation;
}
float SpotLight::getConstantAttenuation() const
{
    return mConstantAttenuation;
}

void SpotLight::setLinearAttenuation(float linearAttenuation)
{
     glLightf(mLightId, GL_LINEAR_ATTENUATION, linearAttenuation);
     mLinearAttenuation = linearAttenuation;
}
float SpotLight::getLinearAttenuation() const
{
    return mLinearAttenuation;
}

void SpotLight::setQuadricAttenuation(float quadricAttenuation)
{
     glLightf(mLightId, GL_QUADRATIC_ATTENUATION, quadricAttenuation);
}
float SpotLight::getQuadricAttenuation() const
{
    return mQuadricAttenuation;
}

void SpotLight::setPosition(const sf::Vector3f& pos)
{
    mPosition = pos;
    GLfloat glPos[4];
    glPos[0] = pos.x;
    glPos[1] = pos.y;
    glPos[2] = pos.z;
    glPos[3] = 1.0f;
    glLightfv(mLightId, GL_POSITION, glPos);
}
sf::Vector3f SpotLight::getPosition() const
{
    return mPosition;
}

void SpotLight::setDirection(const sf::Vector3f& direction)
{
    dinf << "Set SPOTLIGHT direction";
    mDirection = direction;
    GLfloat glDir[3];
    glDir[0] = direction.x;
    glDir[1] = direction.y;
    glDir[2] = direction.z;
    glLightfv(mLightId, GL_SPOT_DIRECTION, glDir);
}
