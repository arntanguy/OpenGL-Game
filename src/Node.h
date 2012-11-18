#ifndef __Node__
#define __Node__

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>

class Node
{
    protected:
        sf::Vector3f mPos;
        std::string mName;

    public:
        Node(std::string name);
        Node(std::string name, float x, float y, float z);
        Node(std::string name, sf::Vector3f pos);
        virtual ~Node();
};

#endif
