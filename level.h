//  Created by Duncan Klug on 4/7/17.
//	Lab 02 Spring 2017
#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Button.h"

using namespace sf;

class Level
{
protected:
  int resW, resH;
  RenderWindow *window;
  int *levelIndex;
  Event *event;
  RectangleShape background;

public:
  Level()
  {
    resW = 800;
    resH = 600;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(*(new Color(255,255,255)));
  }
  Level(int rw, int rh, RenderWindow &win, Event &ev, int &lin)
  {
    resW = rw;
    resH = rh;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(*(new Color(255,255,255)));
    window = &win;
    levelIndex = &lin;
    event = &ev;
  }
  virtual ~Level()
  {

  }

  virtual void draw()
  {
    //All levels must be drawn
  }

  void checkExit()
  {
    //Close when Escape Key is pressed
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
      window->close();
    }
  }

};
#endif
