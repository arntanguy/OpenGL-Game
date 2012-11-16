#ifndef __RessourcesManager__
#define __RessourcesManager__

#include <SFML/Graphics/Image.hpp>
#include <map>
#include <stdexcept>
#include <iostream>

/**
 * @brief Manages the ressources.
 * Each ressource is identified by an unique string (path, name...)
 */
class RessourcesManager
{
    private:
        inline explicit RessourcesManager() {}
        inline ~RessourcesManager() {}
        inline explicit RessourcesManager(RessourcesManager const&) {}
        inline RessourcesManager& operator=(RessourcesManager const&) { return *this; }

    protected:
        std::map<std::string, sf::Image*> mImages;

    public:
        inline static RessourcesManager& getInstance() {
            static RessourcesManager instance;
            return instance;
        }

        bool loadImage(const std::string& path);
        sf::Image* getImage(const std::string& path) const;
};


#endif
