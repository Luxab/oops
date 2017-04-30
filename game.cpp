//  Created by Duncan Klug on 4/6/17.
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
  g.onResizeWindow = onResizeWindowFunc;

  g.setup();

  play();
  return 0;
}

#endif
