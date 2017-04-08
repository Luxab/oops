//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include "mainMenu.h"
#include "scoreBoard.h"

using namespace sf;

enum gameState {
    PLAY,
    VICTORY,
    GAMEOVER
} gameState = PLAY;

class Game
{
  int resW, resH; //Screen resolution
  int level = 0;

  Font gameFont;
  Event event;
  Texture BGTexture;
  RenderWindow window;

  //Levels:
  mainMenu *mm;
  scoreBoard *sb;

public:
  Game()
  {
    resW = 800;
    resH = 600;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    mm = new mainMenu(resW,resH,window,level,gameFont,BGTexture);
    sb = new scoreBoard(resW,resH,window,level,gameFont,BGTexture);
  }
  Game(int rw, int rh)
  {
    resW = rw;
    resH = rh;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    mm = new mainMenu(resW,resH,window,level,gameFont,BGTexture);
    sb = new scoreBoard(resW,resH,window,level,gameFont,BGTexture);
  }
  ~Game()
  {

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

  void play()
  {
    window.create(VideoMode(resW,resH), "Game Template");
    window.setFramerateLimit(60);
    window.clear();

    while (window.isOpen())
    {
      // Game state
      if (level == 0)
      {
        //Main menu
        mm->draw();
      }
      if (level ==1)
      {
        //Scoreboard
        sb->draw();
      }

      Sprite background(BGTexture);

      window.display();

      checkExit();

      window.draw(background);
    }
  }

};
