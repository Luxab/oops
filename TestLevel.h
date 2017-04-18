//  Created by Duncan Klug on 4/8/17.
//  Lab 02 Spring 2017
#include "Level.h"
#include "Player.h"

class TestLevel : public Level
{
  Player p;
  Texture playerTexture;

public:
  TestLevel(RenderWindow &win, Event &ev, changeLevel cl) : Level(win,ev,cl)
  {
    IntRect playerRectangle(0,0,100,100);
    playerTexture.loadFromFile("Placeholder.png");
    p = Player(playerTexture, playerRectangle, 10, 100);
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
    //BGTexture->loadFromFile("MMBG.png");
    window->draw(p);
    p.draw();

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      //*levelIndex = 0;
    }
    else
    {

    }
  }
};
