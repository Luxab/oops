//  Created by Duncan Klug on 4/8/17.
//  Lab 02 Spring 2017
#include "Level.h"

class TestLevel : public Level
{

public:
  TestLevel(RenderWindow &win, Event &ev, changeLevel cl) : Level(win,ev,cl)
  {

  }
  ~TestLevel()
  {

  }

//--------------------------------IMPLEMENTATION-----------------------------------//

  void resize()
  {
    
  }

  void draw()
  {
    window->draw(background); //draw background first!
    //BGTexture->loadFromFile("MMBG.png");

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      //*levelIndex = 0;
    }
    else
    {

    }
  }
};
