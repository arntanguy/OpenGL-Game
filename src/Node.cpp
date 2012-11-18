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

Node::~Node()
{
    std::cerr << "Node destructor" << std::endl;
}
