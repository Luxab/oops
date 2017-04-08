//  Created by Duncan Klug on 4/7/17.
//	Lab 02 Spring 2017
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
  Texture *BGTexture;
  int *levelIndex;
  Event *event;

public:
  Level()
  {
    resW = 800;
    resH = 600;
  }
  Level(int rw, int rh, RenderWindow &win, Event &ev, int &lin, Texture &tin)
  {
    resW = rw;
    resH = rh;
    window = &win;
    BGTexture = &tin;
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

};
