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

  //Levels:
  //mainMenu and scoreboard are special because they need gameFont
  //testLevel doesn't need anything extra in contructor
  //eventually regular levels could be put in a vector, initialized that way.

public:
  void (*changeLevel)(std::string);
  void (*startNewGame)();
  void (*onResizeWindow)();

  Event event;
  gWindow window;
  Vector2u winSize;
  Font gameFont;

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

    levels["intro"]   = new IntroScreen(window,event,changeLevel,gameFont);
    levels["main"]   = new MainMenu(window,event,changeLevel,startNewGame,gameFont);
    levels["scores"] = new ScoreBoard(window,event,changeLevel,gameFont);
    levels["pause"]  = new PauseMenu(window,event,changeLevel,gameFont);
    levels["victory"] = new VictoryScreen(window,event,changeLevel,gameFont);

    currLevel = levels["intro"];
  }

  void play();
};
#endif
