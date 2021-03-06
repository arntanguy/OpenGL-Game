/**
 * OpenGL Game
 * Author: Arnaud TANGUY
 */

#include "Viewer.h"
#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

void reshapeFunc(int width, int height)
{
  // set the new width/height values
  //theViewer.setDimension(width, height);
}

int main(int argc, char *argv[])
{
    sf::Window window;
    /**
     * Init Glew. Used for shaders mostly
     */
    glutInit(&argc, argv);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    Viewer theViewer = Viewer::getInstance();
    // create our view instance
    if(!theViewer.onCreate(argc, argv))
    {
        std::cerr << "Creation of the viewer failed." << std::endl;
        return -1;
    }

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    if(theViewer.getFullscreen()) {
        window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), theViewer.getCaption(), sf::Style::Fullscreen);
    } else {
        window.create(sf::VideoMode(theViewer.getWidth(), theViewer.getHeight(), desktop.bitsPerPixel), theViewer.getCaption());
    }
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
            switch(event.type) {
                case sf::Event::Closed:
                    // end the program
                    running = false;
                    break;
                case sf::Event::Resized:
                    // adjust the viewport when the window is resized
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;
                case sf::Event::MouseMoved:
                    theViewer.onMouseMove(event.mouseMove);
                    break;
                case sf::Event::MouseButtonPressed:
                    theViewer.onMouseButtonDown(event.mouseButton);
                    break;
                case sf::Event::MouseButtonReleased:
                    theViewer.onMouseButtonUp(event.mouseButton);
                    break;

                case sf::Event::KeyPressed:
                    theViewer.onKey(event.key);
                    break;
                default:
                    break;
            }
        }

        // Draw and clear buffers
        theViewer.onRender();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
    return 0;
}
