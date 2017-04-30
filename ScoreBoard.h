//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Level.h"
#include <vector>
#include <fstream>

using namespace sf;

class ScoreBoard : public Level
{
  Font gameFont;
  std::vector<Button> scores;
  Button *titleBanner;
  Button *backButton;

public:
  ScoreBoard(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
  {
    gameFont = fin;
    drawButtons();
  }
  ~ScoreBoard()
  {

  }

//---------------------------------------------------------------------------------//

  void drawButtons()
  {
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
  }

  void resize()
  {
    drawButtons();
  }

  void draw()
  {
    checkWindowSize();
    // Draw scores to screen
    for (unsigned int i = 0; i<scores.size(); i++)
    {
      scores[i].draw(*window);
    }

    titleBanner->draw(*window);
    backButton->draw(*window);

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      if(backButton->contains(mouseX,mouseY))
        cl("main");
    }
    else
    {
      backButton->checkHover(mouseX,mouseY);
    }
  }

  void writeNewScore (int score)
  {
    
  }

};
#endif
