#include "EnvironmentSettings.h"

EnvironmentSettings::~EnvironmentSettings()
{
}

/**
 * @brief Sets the directional vector of the wind
 *
 * @param windDirection
 *      The wind direction vector
 */
void EnvironmentSettings::setWindDirection(const sf::Vector3f& windDirection)
{
    mWindDirection = windDirection;
}

sf::Vector3f EnvironmentSettings::getWindDirection()
{
    return mWindDirection;
}

/**
 * @brief Sets the wind strength
 *
 * @param strength
 *      Strength of the wind between 0 and 1
 */
void EnvironmentSettings::setWindStrength(float windStrength)
{
    mWindStrength = windStrength;
}

float EnvironmentSettings::getWindStrength() const
{
    return mWindStrength;
}
