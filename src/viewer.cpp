//----------------------------------------------------------------------------//
// viewer.cpp                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#include "viewer.h"
#include "tick.h"
#include <string.h>
#include "TerrainPage.h"
#include <GL/glut.h>

#define WALK_SPEED 10

Viewer::Viewer()
{
    m_width = 640;
    m_height = 480;
    m_bFullscreen = false;
    m_mouseX = 0;
    m_mouseY = 0;
    m_tiltAngle = -70.0f;
    m_twistAngle = -45.0f;
    m_distance = 270.0f;
    m_bLeftMouseButtonDown = false;
    m_bRightMouseButtonDown = false;
    m_lastTick = 0;
    m_bPaused = false;
    m_cursorSize = 3;
    m_scale = 1;
    m_wireFrame = false;

    mCamera = new Camera("Camera", sf::Vector3f(600, 30, 600), sf::Vector3f(0, 0, 0), sf::Vector3i(0, 1, 0));

    axisEntity = new AxisEntity(100);
    axisNode = new Node("test", 0, 0, 0);
    axisNode->attachEntity(axisEntity);
}

Viewer::~Viewer()
{
}

std::string Viewer::getCaption()
{
    return "Computer Graphics Game";
}

bool Viewer::getFullscreen()
{
    return m_bFullscreen;
}

int Viewer::getHeight()
{
    return m_height;
}

int Viewer::getWidth()
{
    return m_width;
}

bool Viewer::onCreate(int argc, char *argv[])
{
    // show some information
    std::cout << "o----------------------------------------------------------------o" << std::endl;
    std::cout << "|                Computer Graphics OpenGL Game                   |" << std::endl;
    std::cout << "|                (C) 2012-2013 Arnaud TANGUY                     |" << std::endl;
    std::cout << "o----------------------------------------------------------------o" << std::endl;
    std::cout << "| This program is free software; you can redistribute it and/or  |" << std::endl;
    std::cout << "| modify it under the terms of the GNU General Public License as |" << std::endl;
    std::cout << "| published by the Free Software Foundation; either version 2 of |" << std::endl;
    std::cout << "| the License, or (at your option) any later version.            |" << std::endl;
    std::cout << "o----------------------------------------------------------------o" << std::endl;
    std::cout << std::endl;

    // parse the command line arguments
    int arg;
    for(arg = 1; arg < argc; arg++)
    {
        // check for fullscreen flag
        if(strcmp(argv[arg], "--fullscreen") == 0) m_bFullscreen = true;
        // check for window flag
        else if(strcmp(argv[arg], "--window") == 0) m_bFullscreen = false;
        // check for dimension flag
        else if((strcmp(argv[arg], "--dimension") == 0) && (argc - arg > 2))
        {
            m_width = atoi(argv[++arg]);
            m_height = atoi(argv[++arg]);
            if((m_width <= 0) || (m_height <= 0))
            {
                std::cerr << "Invalid dimension!" << std::endl;
                return false;
            }
        }
        // check for help flag
        else if(strcmp(argv[arg], "--help") == 0)
        {
            std::cerr << "Usage: " << argv[0] << " [--fullscreen] [--window] [--dimension width height] [--help] model-configuration-file" << std::endl;
            return false;
        }
    }
    return true;
}

void Viewer::onIdle()
{
    // get the current tick value
    unsigned int tick;
    tick = Tick::getTick();

    // calculate the amount of elapsed seconds
    float elapsedSeconds;
    elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

    // update the model if not paused
    if(!m_bPaused)
    {
        // update models
    }

    // current tick will be last tick next round
    m_lastTick = tick;

    // update the screen
}

bool Viewer::onInit() {
    loadTerrain();
    return true;
}

void Viewer::loadTerrain()
{
    mTerrain = new Terrain(500, 2, 1, 2);

    mTerrainPage = new TerrainPage("./assets/terrain/heightmap.bmp", 400, 400, 60, 2);
    sf::Image *mixmap = mTerrainPage->getMixmap();
    for(int x = 0; x < mixmap->getSize().x; x++) {
        for(int y = 0; y < mixmap->getSize().y; y++) {
            float height = mTerrainPage->getHeight(x, y);
            if(height < 5)
                mixmap->setPixel(x, y, sf::Color(255, 0, 0, 0));
            else if (height >= 5 && height < 10)
                mixmap->setPixel(x, y, sf::Color(150, 150, 0, 0));
            else if (height >= 10 && height < 20)
                mixmap->setPixel(x, y, sf::Color(0, 150, 150, 0));
            else if (height >= 20 && height < 30)
                mixmap->setPixel(x, y, sf::Color(0, 0, 150, 50));
            else
                mixmap->setPixel(x, y, sf::Color(0, 0, 0, 255));

        }
    }
    mTerrainPage->setMixmap();
    mTerrainPage->generateVertices();
    mTerrainPage->startWave(true);
}

void Viewer::onKey(const sf::Event::KeyEvent& key)
{
    switch(key.code) {
        case sf::Keyboard::Escape:
            exit(0);
            break;
        case sf::Keyboard::Z:
            mCamera->moveCamera(WALK_SPEED);
            break;
        case sf::Keyboard::S:
            mCamera->moveCamera(-WALK_SPEED);
            break;
        case sf::Keyboard::Q:
            mCamera->strafeCamera(-WALK_SPEED);
            break;
        case sf::Keyboard::D:
            mCamera->strafeCamera(WALK_SPEED);
            break;
        case sf::Keyboard::J:
            mCamera->moveCameraUp(-WALK_SPEED/2);
            break;
        case sf::Keyboard::K:
            mCamera->moveCameraUp(WALK_SPEED/2);
            break;
        case sf::Keyboard::W:
            if(m_wireFrame) {
                // Normal
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            } else {
                // Wireframe
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            }
            m_wireFrame = !m_wireFrame;
            break;
    }
}

void Viewer::onMouseButtonDown(const sf::Event::MouseButtonEvent& mouseEvent)
{
    float x = mouseEvent.x;
    float y = getHeight() - mouseEvent.y - 1;
    if(mouseEvent.button == sf::Mouse::Left) {
        m_bLeftMouseButtonDown = true;
    }
    if(mouseEvent.button == sf::Mouse::Right)
    {
        m_bRightMouseButtonDown = true;
    }

    // update internal mouse position
    m_mouseX = x;
    m_mouseY = y;
}

void Viewer::onMouseButtonUp(const sf::Event::MouseButtonEvent& mouseEvent)
{
    float x = mouseEvent.x;
    float y = getHeight() - mouseEvent.y - 1;
    // update mouse button states
    if(mouseEvent.button == sf::Mouse::Left) {
        m_bLeftMouseButtonDown = false;
    }
    if(mouseEvent.button == sf::Mouse::Right)
    {
        m_bRightMouseButtonDown = false;
    }

    // update internal mouse position
    m_mouseX = x;
    m_mouseY = y;
}

void Viewer::onMouseMove(const sf::Event::MouseMoveEvent& mouseEvent)
{ float x = mouseEvent.x; float y = getHeight() - mouseEvent.y - 1;
    // update twist/tilt angles
    if(m_bLeftMouseButtonDown)
    {
        // calculate new angles
        m_twistAngle += (float)(x - m_mouseX);
        m_tiltAngle -= (float)(y - m_mouseY);
    }

    // update distance
    if(m_bRightMouseButtonDown)
    {
        // calculate new distance
        m_distance -= (float)(y - m_mouseY) / 3.0f;
        if(m_distance < 0.0f) m_distance = 0.0f;
    }

    // update internal mouse position
    m_mouseX = x;
    m_mouseY = y;
    mCamera->mouseMove(sf::Vector2f(x, y), sf::Vector2f(m_width, m_height));
}


void Viewer::onRender()
{
    // clear all the buffers
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, m_scale * 5.0, m_scale * 10000.0);

    // set the model transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    sf::Vector3f pos = mCamera->getPosition();
    sf::Vector3f view = mCamera->getView();
    sf::Vector3<int> up = mCamera->getUp();
    gluLookAt(pos.x, pos.y, pos.z, view.x, view.y, view.z, up.x, up.y, up.z);

    // light attributes
    //const GLfloat light_ambient[]  = { 0.9f, mCamera->getPosition().z/m_height, 0.9f, 1.0f };
    //const GLfloat light_diffuse[]  = { 0.9f, 0.1f, 0.f, 1.0f };
    //const GLfloat light_specular[] = { 0.9f, mCamera->getPosition().x/m_width, 0.1f, 1.0f };
    const GLfloat light_ambient[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
    const GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    const GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    GLfloat emerald_ambient[] =
    {0.215, 0.215, 0.215}, emerald_diffuse[] =
    {0.7568, 0.7568, 0.7568}, emerald_specular[] =
    {0.727811, 0.727811, 0.727811}, emerald_shininess = 76.8;

    // setup the light attributes
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);

    // set the light position
    GLfloat lightPosition[] = { -1.0f, 1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emerald_shininess);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPushMatrix();
        axisNode->render();
        mTerrainPage->render();
        //glTranslatef(1000,0,0);
        //mTerrainPage->render();
        //glTranslatef(1000,0,0);
        //mTerrainPage->render();

    glPopMatrix();
    //glPushMatrix();
    //      glTranslatef(0, 0, 499);
    //    mTerrainPage->render();
    //glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    // render the cursor
    glPushMatrix();
        renderCursor();
    glPopMatrix();
}

void Viewer::onShutdown()
{
    // destroy model instance
    // destroy core model instance
}

void Viewer::renderCursor()
{
    // switch to orthogonal projection for the cursor
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // render the cursor
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float offset = m_cursorSize/4;
    float y, x;
    float xCenter = m_mouseX;
    float yCenter = m_mouseY;

    glBegin(GL_TRIANGLES);
    // Bottom
    y = yCenter - offset;
    glColor4f(0,0,0,0.8);
    glVertex2i(xCenter-m_cursorSize, y);
    glVertex2i(xCenter+m_cursorSize, y);
    glColor4f(1,1,1,0.8);
    glVertex2i(xCenter, y - m_cursorSize*2);

    // Top
    y = yCenter + m_cursorSize*2 + 2*offset;
    glColor4f(0,0,0,0.8);
    glVertex2i(xCenter-m_cursorSize, y);
    glVertex2i(xCenter+m_cursorSize, y);
    glColor4f(1,1,1,0.8);
    glVertex2i(xCenter, y + m_cursorSize*2);

    // Left
    glColor4f(0,0,0,0.8);
    x = xCenter - offset - m_cursorSize;
    y = yCenter + m_cursorSize + offset;
    glVertex2i(x, y-m_cursorSize);
    glVertex2i(x, y+m_cursorSize);
    glColor4f(1,1,1,0.8);
    glVertex2i(x-m_cursorSize*2, y);

    // Right
    glColor4f(0,0,0,0.8);
    x = xCenter + offset + m_cursorSize;
    y = yCenter + m_cursorSize + offset;
    glVertex2i(x, y-m_cursorSize);
    glVertex2i(x, y+m_cursorSize);
    glColor4f(1,1,1,0.8);
    glVertex2i(x+m_cursorSize*2, y);
    glEnd();

    glDisable(GL_BLEND);
}

void Viewer::setDimension(int width, int height)
{
    // store new width and height values
    m_width = width;
    m_height = height;

    // set new viewport dimension
    glViewport(0, 0, m_width, m_height);
}

