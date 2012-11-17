#ifndef __Node__
#define __Node__

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class Node
{
    protected:
        sf::Vector3<float> mPos;
        std::string mName;

    public:
        Node(std::string name);
        Node(std::string name, float x, float y, float z);
        Node(std::string name, sf::Vector3<float> pos);
        virtual ~Node();
};

#endif
