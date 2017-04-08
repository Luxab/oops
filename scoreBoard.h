//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include "level.h"
#include <vector>
#include <fstream>

using namespace sf;

class scoreBoard : public Level
{
  Font gameFont;
  bool init = false;
  std::vector<Button> scores;
  Button *titleBanner;
  Button *backButton;

public:
  scoreBoard(int rw, int rh, RenderWindow &win, Event &ev, int &level, Font fin) : Level(rw,rh,win,ev,level)
  {
    gameFont = fin;
  }
  ~scoreBoard()
  {

  }

//---------------------------------------------------------------------------------//

  void initSB()
  {
    window->clear();

    titleBanner= new Button(0,resH/12,gameFont,"Scores",(.06*resW));
    titleBanner->centerWidth(resW);

    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());

    std::ifstream textScores("scores.txt");

    for (int i = 0; i<9; i++)
    {
      if(textScores.eof())
      {
        break;
      }

      std::string s;
      std::getline(textScores,s);

      Button b = *(new Button(0,(i+3)*resH/12,gameFont,s,(.03*resW)));
      b.centerWidth(resW);
      scores.push_back(b);
    }

    init = true;
  }
  void draw()
  {
    if(!init)
    {
      initSB();
    }

    for (int i = 0; i<scores.size(); i++)
    {
      scores[i].draw(*window);
    }
    titleBanner->draw(*window);
    backButton->draw(*window);

    //BGTexture->loadFromFile("MMBG.png");

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
