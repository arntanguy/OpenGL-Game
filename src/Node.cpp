#include "Node.h"

Node::Node(std::string name) : mName(name)
{
}

Node::Node(std::string name, float x, float y, float z)
{
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}

Node::Node(std::string name, sf::Vector3f pos)
{
    mPos = pos;
}

void Node::init()
{
    mEntity = 0;
}

Node::~Node()
{
    std::cerr << "Node destructor" << std::endl;
}


/**
 * @brief Attach an entity (3D model) to the node
 *
 * @param entity
 *      The entity to attach
 */
void Node::attachEntity(Entity *entity)
{
    mEntity = entity;
}

bool Node::render()
{
    if(mEntity != 0)
        return mEntity->render();
    else
        return false;
}
