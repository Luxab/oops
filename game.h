//  Created by Duncan Klug on 3/19/17.
//	Lab 02 Spring 2017
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

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
    screenInd=0;
    resW = 800;
    resH = 600;
    xmult = resW/3;
    ymult = resH/3;
    gameFont.loadFromFile("Raleway-Regular.ttf");
  }
  Game(int rw, int rh)
  {
    screenInd=0;
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

bool contains(int mx, int my, int x, int y, int w, int h)
{
  return (mx>x && mx<x+w && my>y && my<y+h);
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

    Text *titleText = new Text("Game Name",gameFont,90);
    titleText->setPosition(25,50);
    window.draw(*titleText);

    //Single Player button
    Text *singText = new Text("Single Player",gameFont,80);
    singText->setPosition(100,250);
    window.draw(*singText);

    //Dual player button
    Text *dualText = new Text("Dual Player",gameFont,80);
    dualText->setPosition(100,350);
    window.draw(*dualText);

    //exit button
    Text *exitText = new Text("Quit",gameFont,50);
    exitText->setPosition(100,500);
    window.draw(*exitText);

    BGTexture.loadFromFile("MMBG.png");
    //drawMain();
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
