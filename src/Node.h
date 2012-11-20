#ifndef __Node__
#define __Node__

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include "Entity.h"
#include <GL/gl.h>
#include "Debug.h"

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
        sf::Vector3f mTranslate;
        sf::Vector3f mRotation;
        std::string mName;
        Entity *mEntity;
        std::unordered_map<std::string, Node *> mChildNodes;

        void init();
        void render(Node *node);

    public:
        Node();
        Node(std::string name);
        Node(std::string name, float x, float y, float z);
        Node(std::string name, sf::Vector3f pos);
        virtual ~Node();

        void setName(const std::string &name) {
            mName = name;
        }
        std::string getName() const {
            return mName;
        }

        void rotate(float x, float y, float z);
        void translate(float x, float y, float z);

        void attachEntity(Entity *entity);
        void addChild(Node *parent);
        virtual bool render();
        virtual bool renderAllChildren();
};

#endif
