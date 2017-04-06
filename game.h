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
  int screenInd;

  Font gameFont;

  Event event;

  Texture BGTexture;

  RenderWindow window;

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
//------------------------------MouseInput Functions----------------------------//
void mouseOnMain()
{
  //----------------Mouse Input--------------------
  if (Mouse::isButtonPressed(Mouse::Left))
  {
    Vector2i mousepos = Mouse::getPosition(window);
    float x = mousepos.x;
    float y = mousepos.y;

    Clock c;
    Time t = seconds(.2);
    while (c.getElapsedTime()!=t)
    {

    }
  }
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

  void mainMenu()
  {
    Color buttonColor(70,70,70);
    //Title text, box behind title for visibility
    mouseOnMain();

    RectangleShape r;
    r.setSize(Vector2f(780,110));
    r.setPosition(15,50);
    r.setFillColor(buttonColor);
    window.draw(r);

    /*
    Text *titleText = new Text("Game Name",gameFont,90);
    titleText->setPosition(25,50);
    window.draw(*titleText);
    */

    BGTexture.loadFromFile("MMBG.png");
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
