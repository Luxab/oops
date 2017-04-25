//  Created by Duncan Klug on 4/7/17.
//	Lab 02 Spring 2017
#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "Button.h"
#include "Projectile.h"
#include "Wave.h"

using namespace sf;

typedef void (*changeLevel)(std::string);

class Level
{
protected:
  int resW, resH;
  RenderWindow *window;
  Event *event;
  RectangleShape background;
  changeLevel cl;

public:
  Clock levelClock;

  Level()
  {
    resW = 800;
    resH = 600;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(Color(255,255,255));
  }
  Level(RenderWindow &win, Event &ev, changeLevel cl)
  {
    // Get width of height of current winow
    resW = win.getSize().x;
    resH = win.getSize().y;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(Color(255,255,255));
    window = &win;
    event = &ev;
    this->cl = cl;
  }
  virtual ~Level()
  {

  }

  virtual void resize()
  {

  }

  void checkWindowSize()
  {
    resW = window->getView().getSize().x;
    resH = window->getView().getSize().y;
    background.setSize(Vector2f(resW,resH));
    //resize();
  }

  virtual void draw()
  {
    //All levels must be drawn
  }

};

//  Created by Duncan Klug on 4/8/17.
//  Lab 02 Spring 2017
#include "Player.h"
#include "Enemy.h"
class TestLevel : public Level
{
  Player p;
  Texture playerTexture;
  float ratio = (float) 2 / 3; // Ratio gameplay:points/text 2/3
  bool waitingForNextLevel = 1;
  Font gameFont;
  RectangleShape *boundingLine; //line at 2/3rds screen width

  std::vector<Wave*> waves;
  int currWaveIndex = -1;

public:
  TestLevel(RenderWindow &win, Event &ev, changeLevel cl,Font fin) : Level(win,ev,cl)
  {
    gameFont = fin;
    //IntRect playerRectangle(0,0,100,100);
    playerTexture.loadFromFile("images/Skateboard_Forward.png");
    FloatRect bbnd = background.getGlobalBounds();
    //FloatRect(bbnd.left,bbnd.top,bbnd.width*ratio,bbnd.height)
    boundingLine = new RectangleShape(Vector2f(0,bbnd.height));
    boundingLine->setPosition(bbnd.width*ratio,0);
    boundingLine->setOutlineColor(Color(0,0,0));
    boundingLine->setOutlineThickness(2);
    p = Player(playerTexture, 10, FloatRect(bbnd.left,bbnd.top,bbnd.width*ratio,bbnd.height));

    // Set up waves
    waves.push_back(new WaveOne);

    readyUpForNextWave();
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
    checkWindowSize();
    window->draw(background); //draw background first!

    window->draw(*boundingLine);

    waves.at(currWaveIndex)->draw(*window);

    window->draw(p); //draw the player
    p.draw(*window); //let the player draw

    if (waitingForNextLevel && Keyboard::isKeyPressed(Keyboard::Space))
    {
        startWave();
        waitingForNextLevel = 0;
    }
  }

  void setCurrentWaveIndex (int wave)
  {
    currWaveIndex = wave;
  }

  int getCurrentWave ()
  {
    return currWaveIndex;
  }

  void readyUpForNextWave ()
  {
    waitingForNextLevel = 1;
    currWaveIndex++;

    if (currWaveIndex >= waves.size())
        // Next wave not found
        // You won!
        std::cout << "No next wave found, you musta won!!!!" << std::endl;

    // TODO: Show this on screen instead
    std::cout << "Press space to start!" << std::endl;
  }

  void startWave ()
  {
    // Find index of current wave
    // Start wave
    waves.at(currWaveIndex)->setBoundaries(Rect<int>(background.getGlobalBounds()));
    waves.at(currWaveIndex)->spawnEnemies();
    std::cout << "Welcome to wave " << currWaveIndex << std::endl;
  }
};

//  Created by Duncan Klug on 4/8/17.
//	Lab 02 Spring 2017
class PauseMenu : public Level
{
  Font gameFont;

  // Buttons
  Button *backButton;

public:
  int pLevelIndex;

  PauseMenu(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
  {
    gameFont = fin;

    // Display a back button
    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());
  }
  ~PauseMenu()
  {

  }

//--------------------------------IMPLEMENTATION-----------------------------------//
  void resize()
  {
    // Display a back button
    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());
  }

  void draw()
  {
    checkWindowSize();
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
      backButton->checkHover(mouseX,mouseY);
  }

};


#endif
