#ifndef __EnvironmentSettings__
#define __EnvironmentSettings__

#include <SFML/Graphics.hpp>

/**
 * @brief Singleton handling environment settings (wind, fog...)
 */
class EnvironmentSettings {
    private:
        inline explicit EnvironmentSettings() {}
        ~EnvironmentSettings();
        inline explicit EnvironmentSettings(EnvironmentSettings const&) {}
        inline EnvironmentSettings& operator=(EnvironmentSettings const&) { return *this; }

        sf::Vector3f mWindDirection;

    public:
        inline static EnvironmentSettings& getInstance() {
            static EnvironmentSettings instance;
            return instance;
        }

        void setWindDirection(const sf::Vector3f& windDirection);
        sf::Vector3f getWindDirection();
};

#endif
