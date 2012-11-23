#include "EnvironmentSettings.h"

EnvironmentSettings::~EnvironmentSettings()
{
}

void EnvironmentSettings::setWindDirection(const sf::Vector3f& windDirection)
{
    mWindDirection = windDirection;
}

sf::Vector3f EnvironmentSettings::getWindDirection()
{
    return mWindDirection;
}
