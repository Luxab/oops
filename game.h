//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include "mainMenu.h"
#include "scoreBoard.h"
#include "testLevel.h"
#include "pauseMenu.h"

using namespace sf;

enum gameState {
    PLAY,
    VICTORY,
    GAMEOVER
} gameState = PLAY;

class Game
{
  int resW, resH; //Screen resolution
  int levelIndex = 0;
  int tempLevelInd = 0;

  Font gameFont;
  Event event;
  RenderWindow window;

  //Levels:
  //mainMenu and scoreboard are special because they need gameFont
  mainMenu *mm;
  scoreBoard *sb;
  pauseMenu *pm;
  //testLevel doesn't need anything extra in contructor
  //eventually regular levels could be put in a vector, initialized that way.
  testLevel *tl;

public:
  Game()
  {
    resW = 800;
    resH = 600;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    mm = new mainMenu(resW,resH,window,event,levelIndex,gameFont);
    sb = new scoreBoard(resW,resH,window,event,levelIndex,gameFont);
    tl = new testLevel(resW,resH,window,event,levelIndex);
    pm = new pauseMenu(resW,resH,window,event,levelIndex,gameFont);
  }
  Game(int rw, int rh)
  {
    resW = rw;
    resH = rh;
    gameFont.loadFromFile("Raleway-Regular.ttf");
    mm = new mainMenu(resW,resH,window,event,levelIndex,gameFont);
    sb = new scoreBoard(resW,resH,window,event,levelIndex,gameFont);
    tl = new testLevel(resW,resH,window,event,levelIndex);
    pm = new pauseMenu(resW,resH,window,event,levelIndex,gameFont);
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
}

//---------------------------------------------------------------------------------//

  void play()
  {
    window.create(VideoMode(resW,resH), "Game Template");
    window.setFramerateLimit(60);
    window.clear();

    while (window.isOpen())
    {
      window.clear();

      if(tempLevelInd!=levelIndex && (levelIndex == 3 || levelIndex ==2))
      {
        pm->pLevelIndex = tempLevelInd;
        pm->levelClock.restart();
        pm->delayBool = false;
        tl->levelClock.restart();
        tl->delayBool = false;
        tempLevelInd = levelIndex;
      }

      // Game state
      if (levelIndex == 0)
      {
        //Main menu
        mm->draw();
      }
      if (levelIndex ==1)
      {
        //Scoreboard
        sb->draw();
      }
      if (levelIndex == 2)
      {
        //testLevel
        tl->draw();
      }
      if (levelIndex==3)
      {
        //Pause menu
        pm->draw();
      }

      window.display();

      checkExit();
    }
  }

};
