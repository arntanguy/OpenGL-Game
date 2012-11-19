#ifndef __Node__
#define __Node__

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include "Entity.h"

/**
 * @brief Nodes are used to construct a graph of relationships between objects in the scene
 * They effectively act as a local coordinate system, relative to the parent node.
 * So if the parent node is rotated by 45 degres, so will the child node :)
 *
 * A node can have an entity attached to it, so that an object can be rendered
 */
class Node
{
    protected:
        sf::Vector3f mPos;
        std::string mName;
        Entity *mEntity;

        void init();

    public:
        Node(std::string name);
        Node(std::string name, float x, float y, float z);
        Node(std::string name, sf::Vector3f pos);
        virtual ~Node();

        void attachEntity(Entity *entity);
        virtual bool render();
};

#endif
