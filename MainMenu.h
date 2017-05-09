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

#ifndef MAINMENU_H
#define MAINMENU_H

#include "Level.h"
#include "BGShapes.h"
using namespace sf;

class MainMenu : public Level
{

  Font gameFont;

  //Main Menu buttons
  Button *titleBanner;
  Button *playButton;
  Button *optionsButton;
  Button *scoreButton;
  Button *quitButton;

  Music *music = new Music; //Main Menu music

  BGShapes *backgroundShapes;

public:
  MainMenu(RenderWindow &win, Event &ev, changeLevel cl, startNewGame sng,Font fin) : Level(win, ev, cl)
  {
    backgroundShapes = new BGShapes(background, win);
    gameFont = fin;
    initLevel();
    this->sng = sng;

    if (!music->openFromFile("audio/Tempest2000/02 Track 2.wav"))
    {
      std::cout << "MUSIC LOAD FROM FILE ERROR" << std::endl;
    }
    music->setLoop(true);
  }
  ~MainMenu()
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

  void initLevel()
  {
    window->clear();
    //Initializes the buttons on the main menu.
    titleBanner= new Button(0,0,gameFont,"Tony",(.075*resW));
    titleBanner->centerWidth(resW);

    playButton = new Button(0,resH/4.5,gameFont,"Play", (.06*resW));
    playButton->centerWidth(resW);

    optionsButton = new Button(0,2*resH/4.5,gameFont,"Options",(.06*resW));
    optionsButton->centerWidth(resW);

    scoreButton = new Button(0,3*resH/4.5,gameFont,"Score Board", (.06*resW));
    scoreButton->centerWidth(resW);

    quitButton = new Button(0,500,gameFont,"Quit",.05*resW);
    quitButton->setY(resH-quitButton->getHeight());
    quitButton->centerWidth(resW);
  }

  void resize()
  {
    initLevel();
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

    window->draw(background); //draw background first!
    backgroundShapes->draw();
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
      if (playButton->contains(mouseX,mouseY))
      {
        music->stop();
        sng();
        cl("test");
      }
      if (optionsButton->contains(mouseX,mouseY))
      {
        // TODO: Add options
        optionsButton->setPosition(optionsButton->getPosition().x - optionsButton->getGlobalBounds().width/2, optionsButton->getPosition().y);
        optionsButton->setText("Coming Soon");
      }
      if (scoreButton->contains(mouseX,mouseY))
      {
        music->stop();
        cl("scores");
      }
      if (quitButton->contains(mouseX,mouseY))
      {
        music->stop();
        window->close();
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

#endif
