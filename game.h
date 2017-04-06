//  Created by Duncan Klug on 3/19/17.
//	Lab 02 Spring 2017
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "TGrid.h"
#include "badAI.h"

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

  TGrid tileGrid;

  Texture BGTexture;

  bool curse; // Which player, curse refers to cursor

  RenderWindow window;

public:
  Game(int rw, int rh)
  {
    screenInd=0;
    resW = rw;
    resH = rh;
    xmult = resW/3;
    ymult = resH/3;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    curse = 0;
  }
  Game()
  {
    screenInd=0;
    resW = 800;
    resH = 600;
    xmult = resW/3;
    ymult = resH/3;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    curse = 0;
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
  if(curse)
  {
    cursor = new Text("1",gameFont,90);
  }
  if (!curse)
  {
    cursor = new Text("1",gameFont,90);
  }
  cursor->setPosition(mousepos.x-15,mousepos.y-100);
  cursor->setFillColor(Color::Black);
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

  void checkWin()
  {
    //Checking to see if one of the players has won
    int winner = tileGrid.checkWin();
    if(winner==1)
    {
      screenInd = 3;
      //std::cout << "X wins!" << std::endl;
    }
    if(winner==2)
    {
      screenInd = 4;
      //std::cout << "O wins!" << std::endl;
    }
  }

  void twoPlayer()
  {
    // TWO PLAYER
    //cursorFollow(window,curse);
    cursorFollow();
    tileGrid.drawPlayerGrid(window);

    mouseOnGrid();
    checkWin();

    BGTexture.loadFromFile("MMBG.png");
    //drawDPGame();
  }

  void onePlayer()
  {
    // SINGLE PLAYER
    badAI josh;

    //cursorFollow(window,curse);
    cursorFollow();
    tileGrid.drawPlayerGrid(window);
    if(curse)
    {
      josh.makeMove(tileGrid,1);
      curse = !curse;
    }
    if (Mouse::isButtonPressed(Mouse::Left) && !curse)
    {
      //mouseOnGrid(window, tileGrid, curse);
      mouseOnGrid();
    }
    // tileGrid.printBoard();
    // tileGrid.printScores();
    checkWin();
    BGTexture.loadFromFile("MMBG.png");
  }

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
    Text *victoryText;
    if (victor==3)
    {
      victoryText = new Text("X wins",gameFont,250);
    }
    else
    {
      victoryText = new Text("O wins",gameFont,250);
    }
    victoryText->setPosition(xmult/3,150);
    window.draw(*victoryText);

    BGTexture.loadFromFile("MMBG.png");
    tileGrid.drawPlayerGrid(window);
  }

  void drawGridBackground()
  {
    for (int row = 0; row<4; row++)
    {
      RectangleShape r = *(new RectangleShape());
      r.setSize(Vector2f(resW,10));
      r.setPosition(0,row*ymult);
      r.setFillColor(Color::Black);
      window.draw(r);
    }
    for (int c = 0; c<4; c++)
    {
      RectangleShape r = *(new RectangleShape());
      r.setSize(Vector2f(10,resH));
      r.setPosition(c*xmult,0);
      r.setFillColor(Color::Black);
      window.draw(r);
    }
  }

  void play()
  {
    window.create(VideoMode(resW,resH), "Game Template");
    window.setFramerateLimit(60);

    window.clear();
    while (window.isOpen())
    {

      if (screenInd == 0)
      {
        mainMenu();
      }
      else if (screenInd == 2)
      {
        twoPlayer();
        drawGridBackground();
      }
      else if (screenInd==1)
      {
        onePlayer();
        drawGridBackground();
      }
      else
      {
        victoryScreen(screenInd);
        drawGridBackground();
      }

      Sprite background(BGTexture);

      window.display();

      checkExit();

      window.draw(background);
    }
  }

};
