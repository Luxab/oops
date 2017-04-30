//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <string>

#include "MainMenu.h"
#include "ScoreBoard.h"
#include "gWindow.h"

using namespace sf;

enum gameState {
  MAIN_MENU,
  PLAY,
  PAUSE,
  VICTORY,
  GAMEOVER
} gameState = PLAY;

// TODO: Only have resolution determined in game.h
class Game : RenderWindow
{
  int resW, resH; //Screen resolution

  Font gameFont;
  //Levels:
  //mainMenu and scoreboard are special because they need gameFont
  //testLevel doesn't need anything extra in contructor
  //eventually regular levels could be put in a vector, initialized that way.

public:
  void (*changeLevel)(std::string);
  void (*onResizeWindow)();

  Event event;
  gWindow window;
  Vector2u winSize;

  // Current level that is shown on screen
  Level *currLevel;
  Level *pauseBufferLevel; // Stores current level during pause screen

  std::unordered_map<std::string, Level*> levels;

  Game()
  {
    resW = 800;
    resH = 600;
    winSize = Vector2u(resW,resH);
  }
  Game(int rw, int rh)
  {
    resW = rw;
    resH = rh;
    winSize = Vector2u(rw,rh);
    gameFont.loadFromFile("Raleway-Regular.ttf");
  }
  ~Game()
  {

  }

//---------------------------------------------------------------------------------//

  // Pause or escape depending on current level
  void escapePressed();

  void setup()
  {
    // Set proper window size
    window.create(VideoMode(resW,resH), "Game Template", Style::Resize);
    window.setResizeFunction(onResizeWindow);
    window.setFramerateLimit(60);

    levels["main"]   = new MainMenu(window,event,changeLevel,gameFont);
    levels["scores"] = new ScoreBoard(window,event,changeLevel,gameFont);
    levels["test"]   = new TestLevel(window,event,changeLevel,gameFont);
    levels["pause"]  = new PauseMenu(window,event,changeLevel,gameFont);

    currLevel = levels["main"];
  }

  void play();
};
#endif
