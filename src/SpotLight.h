#ifndef __SpotLight__
#define __SpotLight__

#include "DirectionalLight.h"

class SpotLight : public DirectionalLight
{
    protected:
        float mExponent;
        float mConstantAttenuation;
        float mLinearAttenuation;
        float mQuadricAttenuation;

        float mCutoff;

        sf::Vector3f mPosition;

    public:
        SpotLight(GLuint lightId);
        ~SpotLight();

        void setCutoff(float cutOff);
        float getCutoff() const;

        void setExponent(float exponent);
        float getExponent() const;

        void setConstantAttenuation(float constantAttenuation);
        float getConstantAttenuation() const;

        void setLinearAttenuation(float linearAttenuation);
        float getLinearAttenuation() const;

        void setQuadricAttenuation(float quadricAttenuation);
        float getQuadricAttenuation() const;

        void setPosition(const sf::Vector3f& pos);
        sf::Vector3f getPosition() const;

        virtual void setDirection(const sf::Vector3f& direction);
};

#endif
