//----------------------------------------------------------------------------//
// viewer.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef VIEWER_H
#define VIEWER_H


#include "Shader.h"
#include "Camera.h"
#include <SFML/Window/Event.hpp>
#include <string>
#include "AxisEntity.h"
#include "Node.h"
#include "Terrain.h"

#include "FlagEntity.h"
#include "EnvironmentSettings.h"


class TerrainPage;

class Viewer
{
// member variables
protected:
  int m_width;
  int m_height;
  bool m_bFullscreen;
  int m_mouseX;
  int m_mouseY;
  float m_tiltAngle;
  float m_twistAngle;
  float m_distance;
  float m_scale;
  bool m_bLeftMouseButtonDown;
  bool m_bRightMouseButtonDown;
  unsigned int m_lastTick;
  bool m_bPaused;
  float m_cursorSize;

  bool m_wireFrame;

  Terrain *mTerrain;
  TerrainPage *mTerrainPage;
  Camera *mCamera;

  AxisEntity *axisEntity;
  Node *axisNode;

  /**
   * =========================================
   * Test objects only...
   * XXX: Remove them when no test is needed anymore
   * Shader used for tested purposes only
   */
  Shader mTestShader;
  void loadTestShader();
  Entity *mTestEntity;
  Texture*mTestTexture;
  Entity *mTestEntity2;
  Texture*mTestTexture2;
  void loadTestEntity();
  sf::Clock mTestClock;
  /**
   * END OF TESTS
   * =============================================
   */


private:
   // Private constructor to create a singleton
   Viewer( );

// constructors/destructor
public:
    static Viewer& getInstance()
    {
        static Viewer    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
    virtual ~Viewer( );

// member functions
public:
  std::string getCaption();
  bool getFullscreen();
  int getHeight();
  int getWidth();
  bool onCreate(int argc, char *argv[]);
  void onIdle();
  bool onInit();
  void onKey(const sf::Event::KeyEvent& key);
  void onMouseButtonDown(const sf::Event::MouseButtonEvent&);
  void onMouseButtonUp(const sf::Event::MouseButtonEvent&);
  void onMouseMove(const sf::Event::MouseMoveEvent&);
  void onRender();
  void onShutdown();
  void setDimension(int width, int height);

  void loadTerrain();
  void loadEnvironmentSettings();

protected:
  void renderCursor();
};

extern Viewer theViewer;

#endif

//----------------------------------------------------------------------------//
