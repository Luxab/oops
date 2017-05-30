/*  Created by:
 *
 *      * Hunter Brown
 *      * Duncan Klug
 *      * Andrew Morgan
 *      * HuiMin Zhang
 *
 *  Copyright (c) 2017 Really Good Software
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "BGShapes.h"
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
  BGShapes *backgroundShapes;

  Music *music = new Music; //Main Menu music

public:
  ScoreBoard(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
  {
    backgroundShapes = new BGShapes(background, win, Vector2f(
                background.getLocalBounds().width/2,background.getLocalBounds().height),
                SHAPES_BLUE);
    gameFont = fin;
    drawButtons();

    if (!music->openFromFile("audio/misc2/Robert_Ellis_-_Crystal_Kick.wav"))
    {
      std::cout << "MUSIC LOAD FROM FILE ERROR" << std::endl;
    }
    music->setLoop(true);
  }
  ~ScoreBoard()
  {

  }

//---------------------------------------------------------------------------------//

  // Called when screen appears
  void didAppear()
  {
    drawButtons();
  }

  void drawButtons()
  {
    // Remove any old buttons
    scores.clear();

    titleBanner= new Button(0,resH/12,gameFont,"Local Scores",(.06*resW));
    titleBanner->centerWidth(resW);

    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());

    std::ifstream textScores("scores.txt");

    if (!textScores)
    {
      std::cout << "Scores do not exist yet" << std::endl;
      return;
    }

    for (int i = 0; !textScores.eof(); i++)
    {
      std::string s;
      std::getline(textScores,s);

      if (s == "")
        continue;

      Button *b = new Button(0,(i+3)*resH/12,gameFont,s,(.03*resW));
      b->setFillColor(Color(0,0,0,0));
      b->centerWidth(resW);
      scores.push_back(*b);
    }
  }

  void resize()
  {
    drawButtons();
  }

  void playMusic()
  {
    if (!music->getStatus())
    {
      music->play();
    }
  }

  void draw()
  {
    playMusic();
    checkWindowSize();
    backgroundShapes->draw();
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
      {
        music->stop();
        cl("main");
      }
    }
    else
    {
      backButton->checkHover(mouseX,mouseY);
    }
  }

};
#endif
