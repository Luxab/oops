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
  int resW;
  int resH;

public:
  Level()
  {
    resW = 800;
    resH = 600;
  }
  Level(int rw, int rh)
  {
    resW = rw;
    resH = rh;
  }
  virtual ~Level()
  {

  }

  virtual void draw()
  {
    //All levels must be drawn
  }

};
