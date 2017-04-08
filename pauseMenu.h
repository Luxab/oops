//  Created by Duncan Klug on 4/8/17.
//	Lab 02 Spring 2017
#include "level.h"
using namespace sf;

class pauseMenu : public Level
{
  bool init = false;
  Button *backButton;
  Font gameFont;

public:
  pauseMenu(int rw, int rh, RenderWindow &win, Event &ev, int &level,Font fin) : Level(rw,rh,win,ev,level)
  {
    gameFont = fin;
  }
  ~pauseMenu()
  {

  }

//---------------------------------------------------------------------------------//

  void initLevel()
  {
    //window->clear();

    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());

    init = true;
  }
  void draw()
  {
    if(!init)
    {
      initLevel();
    }
    //window->draw(background); //draw background first!
    backButton->draw(*window);

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      if(backButton->contains(mouseX,mouseY))
      {
        *levelIndex = 0;
      }
    }
    else
    {
      backButton->checkHover(mouseX,mouseY);
    }
  }

};
