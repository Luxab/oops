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

  Clock backgroundCirclesClock;                     // Timer to spawn new background circles
  std::vector<CircleShape*> backgroundCircles;      // Vector to hold all background circles
  double backgroundCirclesCooldown = 400;           // Cooldown for spawning new circles
  double backgroundCirclesScaleAmt = 5;             // Rate at which circles scale

public:
  MainMenu(RenderWindow &win, Event &ev, changeLevel cl, startNewGame sng,Font fin) : Level(win, ev, cl)
  {
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
    titleBanner= new Button(0,0,gameFont,"Tony Phawk: The Last Ride",(.075*resW));
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

  void drawBackgroundCircles()
  {
    FloatRect bgDim = background.getGlobalBounds();

    // Add a new circle every few hundred milliseconds
    if (backgroundCirclesClock.getElapsedTime().asMilliseconds() > backgroundCirclesCooldown)
    {
      // Generate new circle
      //

      // Random vars
      std::random_device rd;
      std::mt19937 rng(rd());

      // Random angle
      std::uniform_real_distribution<double> uni(-M_PI/4,M_PI/4);
      auto random_angle = uni(rng);

      std::uniform_int_distribution<int> uni_color_r(100,255);
      std::uniform_int_distribution<int> uni_color_g(0,55);
      std::uniform_int_distribution<int> uni_color_b(0,55);
      auto random_r = uni_color_r(rng);
      auto random_g = uni_color_g(rng);
      auto random_b = uni_color_b(rng);

      CircleShape *c = new CircleShape(0,30);
      c->setPosition(bgDim.width/2,0);
      c->setFillColor(Color(random_r, random_g, random_b));
      backgroundCircles.push_back(c);

      // Reset timer
      backgroundCirclesClock.restart();
    }
    
    // Grow every circle in the vector
    for (CircleShape *c : backgroundCircles)
    {
      // Draw each circle
      window->draw(*c);
      c->setRadius(c->getRadius() + backgroundCirclesScaleAmt);
      //c->setPosition(bgDim.width/2 - c->getRadius(),bgDim.height/2 - c->getRadius());
      c->setPosition(bgDim.width/2 - c->getRadius(),0 - c->getRadius());
    }

    for (int i = 0; i < backgroundCircles.size();)
    {
      CircleShape *c = backgroundCircles.at(i);

      // Delete circles that are too large
      if (c->getRadius() > bgDim.width)
        backgroundCircles.erase(backgroundCircles.begin() + i);
      else
        i++;
    }
  }

  void draw()
  {
    playMusic();
    checkWindowSize();

    window->draw(background); //draw background first!
    drawBackgroundCircles();
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
        music->stop();
        window->close();
      }
      if (scoreButton->contains(mouseX,mouseY))
      {
        music->stop();
        cl("scores");
      }
      if (playButton->contains(mouseX,mouseY))
      {
        music->stop();
        sng();
        cl("test");
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
