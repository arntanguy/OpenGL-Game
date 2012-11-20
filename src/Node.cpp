#include "Node.h"

Node::Node()
{
}

Node::Node(std::string name) : mName(name)
{

}

Node::Node(std::string name, float x, float y, float z)
{
    mName = name;
    mTranslate.x = x;
    mTranslate.y = y;
    mTranslate.z = z;
}

Node::Node(std::string name, sf::Vector3f pos)
{
    mName = name;
    mTranslate = pos;
}

void Node::init()
{
    mEntity = 0;
}

Node::~Node()
{
}

void Node::addChild(Node *node)
{
    mChildNodes[node->getName()] = node;
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


void Node::rotate(float x, float y, float z)
{
    mRotation.x = x;
    mRotation.y = y;
    mRotation.z = z;
}

void Node::translate(float x, float y, float z)
{
    mTranslate.x = x;
    mTranslate.y = y;
    mTranslate.z = z;
}

bool Node::render()
{
    dinf << "Rendering " << mName << std::endl;
    if(mEntity != 0)
        return mEntity->render();
    else
        return false;
}

sf::Vector3f getPosition()
{
    GLdouble modelview[16];
    sf::Vector3f pos;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    pos.x = modelview[12]/modelview[15];
    pos.y = modelview[13]/modelview[15];
    pos.z = modelview[14]/modelview[15];

    //position[0]=modelview[12];///position[3];
    //position[1]=modelview[13];///position[3];
    //position[2]=modelview[14];///position[3];
    //position[3]=modelview[15];///position[3];
    return pos;
}
//TODO
bool Node::renderAllChildren()
{

            //glTranslatef(0.0,0.0,-1000.0);
            //glPushMatrix();
            //    glTranslatef(200,0,0);
            //    glRotatef(1*tetrahedronAngle/4.f,0,1,0);
            //    glRotatef(-1*tetrahedronAngle/3.f,1,0,0);
            //    glRotatef(-1*tetrahedronAngle/2.f,0,0,1);
            //    cubeModel->draw();
            //glPopMatrix();
    // Save matrix current matrix
    glPushMatrix();
        // Apply translation to current node, and all child nodes
        glTranslatef(mTranslate.x, mTranslate.y, mTranslate.z);

        // Recursively render child nodes
        std::unordered_map<std::string, Node *>::const_iterator it;
        for (it = mChildNodes.begin(); it != mChildNodes.end(); it++) {

            glPushMatrix();
                (it->second)->renderAllChildren();
            glPopMatrix();
        }
        render();
    glPopMatrix();
}
