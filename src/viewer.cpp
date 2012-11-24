/******************************************************************************
     Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
*                                                                             *
* This program is free software; you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation; either version 2 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc.,     *
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 ******************************************************************************/

#include <GL/glew.h>
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

void Viewer::loadTestEntity()
{
    mTestTexture = RessourcesManager::getInstance().loadTexture("assets/textures/bzh_flag.gif");
    mTestEntity = new FlagEntity(mTestTexture, mTestTexture->getImage()->getSize().x, mTestTexture->getImage()->getSize().y, 8);
    mTestEntity->generate();

    //mTestTexture2 = new Texture();
    //mTestTexture2->loadTexture("assets/crate.jpg");
    //mTestEntity2 = new FlagEntity(mTestTexture, mTestTexture->getImage()->getSize().x, mTestTexture->getImage()->getSize().y, 8);
    //mTestEntity->generate();
}
void Viewer::loadTestShader()
{
    mTestShader.loadVertexShader("assets/shaders/vertex/flag.glsl");
    mTestShader.loadFragmentShader("assets/shaders/fragment/render_one_texture.glsl");
}

bool Viewer::onInit() {
    loadEnvironmentSettings();
    loadTerrain();
    loadTestEntity();
    loadTestShader();
    return true;
}

void Viewer::loadEnvironmentSettings()
{
    EnvironmentSettings::getInstance().setWindDirection(sf::Vector3f(1,0,0));
    EnvironmentSettings::getInstance().setWindStrength(1);
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
            else if (height < 10)
                mixmap->setPixel(x, y, sf::Color(50, 200, 0, 0));
            else if (height < 20)
                mixmap->setPixel(x, y, sf::Color(0, 150, 150, 0));
            else if (height < 30)
                mixmap->setPixel(x, y, sf::Color(0, 0, 150, 50));
            else
                mixmap->setPixel(x, y, sf::Color(0, 0, 0, 255));

        }
    }
    mTerrainPage->setMixmap();
    mTerrainPage->generateVertices();
    mTerrainPage->startWave(false);


    /**
     * Define directional or positional light
     * */
    float Al[4] = {1.f, 0.8f, 0.8f, 1.0f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, Al );

    float Dl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, Dl );

    float Sl[4] = {1.0f, 0.0f, 0.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_SPECULAR, Sl );

    float Am[4] = {0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Am );

    float Dm[4] = {0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dm );

    float Sm[4] = {0.6f, 0.6f, 0.6f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Sm );

    float f = 60.0f;
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, f );

    // DIRECTIONAL LIGHT with direction (-1, 0, 1)
    // Projected with all parallels rays from infinity (and beyond)
    GLfloat directional_light_position[] = { -1 , -1 , -1, 0 };
    // POSITIONAL LIGHT
    // Light emitted from position (-1,0,1), emitting in all directions
    GLfloat positional_light_position[] = {-1 , 0 , 1, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, directional_light_position);
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
        case sf::Keyboard::B:
            static float i=0;
            EnvironmentSettings::getInstance().setWindDirection(sf::Vector3f(1,0,i+=0.1));

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

    // XXX: Need of Z-Depth sorting to get alpha blending right!!
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Sun
    //glShadeModel( GL_SMOOTH );

    glPushMatrix();
        //axisNode->render();
        mTerrainPage->render();
        //mTerrainPage->render();
    glPopMatrix();

    glPushMatrix();
        axisNode->render();
        mTestShader.enable();
        mTestShader.setFloat("waveTime", mTestClock.getElapsedTime().asSeconds()/10);
        mTestShader.setVec3("windDirection", EnvironmentSettings::getInstance().getWindDirection());
        mTestShader.setFloat("windStrength", EnvironmentSettings::getInstance().getWindStrength());
        mTestShader.setFloat("maxAmplitude", 10);
        mTestShader.setVec3("origin", sf::Vector3f(0,0,0));
        mTestShader.setFloat("nbSquares", dynamic_cast<FlagEntity*>(mTestEntity)->getNbSquares());
        mTestShader.setFloat("width", dynamic_cast<FlagEntity*>(mTestEntity)->getWidth());
        mTestShader.bindTexture(mTestTexture, "tex");
        mTestEntity->render();
        mTestShader.disable();
    glPopMatrix();

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

