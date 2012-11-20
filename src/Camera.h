#ifndef __Camera__
#define __Camera__

#include "Node.h"
#include <math.h>
#include <iostream>

class Camera : public Node
{
    protected:
        sf::Vector3f mView;
        sf::Vector3i mUp;

    public:
        Camera(const std::string& name);
        Camera(const std::string& name, float x, float y, float z);
        Camera(const std::string& name, sf::Vector3f pos);
        Camera(const std::string& name, sf::Vector3f pos, sf::Vector3f view, sf::Vector3i up);
        ~Camera();

        void moveCamera(float speed);
        void strafeCamera(float speed);
        void moveCameraUp(float speed);
        void rotateView(float speed);
        void mouseMove(const sf::Vector2f& mousePos, const sf::Vector2f& windowSize);


        void setPosition(sf::Vector3f pos);
        void setView(sf::Vector3f view);
        inline sf::Vector3f getPosition() const
        { return mTranslate; }
        inline sf::Vector3f getView() const
        { return mView; }
        inline sf::Vector3i getUp() const
        { return mUp; }
};

#endif
