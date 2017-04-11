//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include "level.h"
using namespace sf;

class mainMenu : public Level
{

  Font gameFont;

  //Main Menu buttons
  Button *titleBanner;
  Button *playButton;
  Button *optionsButton;
  Button *scoreButton;
  Button *quitButton;


  bool MMI = false;

public:
  mainMenu(int rw, int rh, RenderWindow &win, Event &ev, int &level, Font fin) : Level(rw,rh,win,ev,level)
  {
    gameFont = fin;
  }
  ~mainMenu()
  {

  }

void cursorFollow()
{
  //  Follows the cursor around with the appropriate player
  Vector2i mousepos = Mouse::getPosition(*window);
  Text *cursor;
  /*
  cursor = new Text("1",gameFont,90);
  cursor->setPosition(mousepos.x-15,mousepos.y-100);
  cursor->setFillColor(Color::Black);
  */
  window->draw(*cursor);
}

//---------------------------------------------------------------------------------//

  void initMM()
  {
    window->clear();
    //Initializes the buttons on the main menu.
    titleBanner= new Button(0,0,gameFont,"Game Template Title",(.075*resW));
    titleBanner->centerWidth(resW);

    playButton = new Button(0,resH/5,gameFont,"Play", (.06*resW));
    playButton->centerWidth(resW);

    optionsButton = new Button(0,2*resH/5,gameFont,"Options",(.06*resW));
    optionsButton->centerWidth(resW);

    scoreButton = new Button(0,3*resH/5,gameFont,"Score Board", (.06*resW));
    scoreButton->centerWidth(resW);

    quitButton = new Button(0,500,gameFont,"Quit",.05*resW);
    quitButton->setY(resH-quitButton->getHeight());
    quitButton->centerWidth(resW);
    MMI = true;
  }
  void draw()
  {
    if(!MMI)
    {
      initMM();
    }

    checkExit();

    window->draw(background); //draw background first!
    titleBanner->draw(*window);
    playButton->draw(*window);
    optionsButton->draw(*window);
    scoreButton->draw(*window);
    quitButton->draw(*window);

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      //std::cout << x << "; " << y << std::endl;
      if (quitButton->contains(mouseX,mouseY))
      {
        window->close();
      }
      if (scoreButton->contains(mouseX,mouseY))
      {
        *levelIndex = 1;
      }
      if (playButton->contains(mouseX,mouseY))
      {
        *levelIndex = 2;
      }
    }
    else
    {
      playButton->checkHover(mouseX,mouseY);
      optionsButton->checkHover(mouseX,mouseY);
      scoreButton->checkHover(mouseX,mouseY);
      quitButton->checkHover(mouseX,mouseY);
    }
  }

};
