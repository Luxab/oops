//  Created by Duncan Klug on 4/8/17.
//	Lab 02 Spring 2017
#include "level.h"
using namespace sf;

class testLevel : public Level
{
  bool init = false;

public:
  testLevel(int rw, int rh, RenderWindow &win, Event &ev, int &level) : Level(rw,rh,win,ev,level)
  {

  }
  ~testLevel()
  {

  }

//---------------------------------------------------------------------------------//

void checkPause()
{
  //  Checks if the player is trying to pause
  window->pollEvent(*event);

  //Close when Escape Key is pressed
  if ((event->type==Event::KeyPressed) && (event->key.code==Keyboard::Escape))
  {
    *levelIndex = 3;
  }
}

  void initLevel()
  {
    window->clear();

    init = true;
  }
  void draw()
  {
    checkPause();
    if(!init)
    {
      initLevel();
    }
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
