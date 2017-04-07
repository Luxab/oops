//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Button.h"

using namespace sf;

enum gameState {
    PLAY,
    VICTORY,
    GAMEOVER
} gameState = PLAY;

class Game
{
  int resW;
  int resH;
  int xmult;
  int ymult;
  int screenInd = 0;

  Font gameFont;

  Event event;

  Texture BGTexture;

  RenderWindow window;

  Button *titleBanner;
  Button *quitButton;
  bool MMI = false;

public:
  Game()
  {
    resW = 800;
    resH = 600;
    xmult = resW/3;
    ymult = resH/3;
    gameFont.loadFromFile("Raleway-Regular.ttf");
  }
  Game(int rw, int rh)
  {
    resW = rw;
    resH = rh;
    xmult = resW/3;
    ymult = resH/3;
    gameFont.loadFromFile("Raleway-Regular.ttf");
  }
  ~Game()
  {

  }

void cursorFollow()
{
  //  Follows the cursor around with the appropriate player
  Vector2i mousepos = Mouse::getPosition(window);
  Text *cursor;
  /*
  cursor = new Text("1",gameFont,90);
  cursor->setPosition(mousepos.x-15,mousepos.y-100);
  cursor->setFillColor(Color::Black);
  */
  window.draw(*cursor);
}

//---------------------------------------------------------------------------------//
//------------------------------KeyboardInput Functions----------------------------//
void checkExit()
{
  //  Checks if the player is trying to exit
  window.pollEvent(event);

  //Close when x on window is pressed
  if (event.type == sf::Event::Closed)
  {
    window.close();
  }

  //Close when Escape Key is pressed
  if ((event.type==Event::KeyPressed) && (event.key.code==Keyboard::Escape))
  {
    window.close();
  }
}

//---------------------------------------------------------------------------------//

  void initMM()
  {
    //Initializes the buttons on the main menu.

    titleBanner= new Button(0,0,gameFont,"Game Template Title",(.075*resW));
    titleBanner->centerWidth(resW);

    quitButton = new Button(0,500,gameFont,"Quit",.05*resW);
    quitButton->setY(resH-quitButton->getHeight());
    quitButton->centerWidth(resW);
    MMI = true;
  }
  void mainMenu()
  {
    if(!MMI)
    {
      initMM();
    }

    titleBanner->draw(window);
    quitButton->draw(window);
    BGTexture.loadFromFile("MMBG.png");

    Vector2i mousepos = Mouse::getPosition(window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      //std::cout << x << "; " << y << std::endl;
      if (quitButton->contains(mouseX,mouseY))
      {
        window.close();
      }
    }
    else
    {
      quitButton->checkHover(mouseX,mouseY);
    }
  }

  void victoryScreen(int victor)
  {
    // Show victory to player
  }

  void play()
  {
    window.create(VideoMode(resW,resH), "Game Template");
    window.setFramerateLimit(60);
    window.clear();

    while (window.isOpen())
    {
      // Game state
      if (screenInd == 0)
      {
        mainMenu();
      }

      Sprite background(BGTexture);

      window.display();

      checkExit();

      window.draw(background);
    }
  }

};
