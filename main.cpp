/**
 * OpenGL Game
 * Author: Arnaud TANGUY
 */

#include "global.h"
#include "viewer.h"
#include <SFML/Window.hpp>

Viewer theViewer = Viewer::getInstance();

void displayFunc()
{
  // render the scene
  theViewer.onRender();
}

void exitFunc()
{
  // shut the viewer instance down
  theViewer.onShutdown();
}

void idleFunc()
{
  // redirect to the viewer instance
  theViewer.onIdle();
}

void keyboardFunc(unsigned char key, int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.onKey(key, x, theViewer.getHeight() - y - 1);
}

void motionFunc(int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.onMouseMove(x, theViewer.getHeight() - y - 1);
}

void reshapeFunc(int width, int height)
{
  // set the new width/height values
  theViewer.setDimension(width, height);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // create our view instance
    if(!theViewer.onCreate(argc, argv))
    {
        std::cerr << "Creation of the viewer failed." << std::endl;
        return -1;
    }

    // register our own exit callback
    atexit(exitFunc);

    sf::Window window(sf::VideoMode(theViewer.getWidth(), theViewer.getHeight()), theViewer.getCaption());
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);

    // set all GLUT modes
    //if(theViewer.getFullscreen()) glutFullScreen();

    // initialize our viewer instance
    if(!theViewer.onInit())
    {
        std::cerr << "Initialization of the viewer failed." << std::endl;
        return -1;
    }


    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            } else if (event.type == sf::Event::MouseMoved) {
                theViewer.onMouseMove(event.mouseMove.x, theViewer.getHeight() - event.mouseMove.y - 1);
            } else if(event.type == sf::Event::MouseButtonPressed) {
                theViewer.onMouseButtonDown(event.mouseButton.button, event.mouseButton.x, theViewer.getHeight() - event.mouseButton.y - 1);
            } else if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i position = sf::Mouse::getPosition();
                theViewer.onMouseButtonUp(event.mouseButton.button, event.mouseButton.x, theViewer.getHeight() - event.mouseButton.y - 1);
            }
        }

        // Draw and clear buffers
        displayFunc();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
    return 0;
}
