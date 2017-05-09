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

#ifndef GAME_CPP
#define GAME_CPP

#include "game.h"

//Aspect ratio: Force 16:9
//If the ratio is > 16:9, game will size to height.
//If the ratio is < 16:9, game will size to width.

//Initial game window size
int resW = 960;
int resH = 540;

Game g(resW,resH);

void onResizeWindowFunc ()
{
  g.window.setView(sf::View(sf::FloatRect(0, 0, g.event.size.width, g.event.size.height)));
}

void changeLevelFunc (std::string levelName)
{
  g.window.clear();
  g.currLevel = g.levels[levelName];
  g.currLevel->didAppear();
}

void startNewGameFunc()
{
  g.levels["test"] = new TestLevel(g.window,g.event,g.changeLevel,g.gameFont);
}

// Pause or escape depending on current level
void escapePressed()
{
  // If in game, escape button pauses
  if (g.currLevel != g.levels["main"] && g.currLevel != g.levels["scores"])
  {
    // Toggle between pause and current level screens
    if (g.pauseBufferLevel && g.currLevel == g.levels["pause"])
    {
      g.currLevel = g.pauseBufferLevel;
    }
    else
    {
      g.pauseBufferLevel = g.currLevel;
      g.currLevel = g.levels["pause"];
    }
    g.currLevel->levelClock.restart();
  }
  else
    // escape button exits
    g.window.close();
}

void play()
{
  g.window.clear();

  while (g.window.isOpen())
  {
    g.window.clear();

    // Check what key is being pressed
    g.window.pollEvent(g.event);

    // Pauses when Escape Key is pressed
    if (Keyboard::isKeyPressed(Keyboard::Escape) && g.currLevel->levelClock.getElapsedTime().asSeconds()>.2)
      escapePressed();

    // Draw current level to screen
    g.currLevel->draw();

    g.window.display();
  }
}

int main()
{
  // Set address value of change level func pointer
  g.changeLevel = changeLevelFunc;
  g.startNewGame = startNewGameFunc;
  g.onResizeWindow = onResizeWindowFunc;

  g.setup();

  play();
  return 0;
}

#endif
