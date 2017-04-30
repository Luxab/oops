//  Created by Duncan Klug on 4/7/17.
//	Lab 02 Spring 2017
#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <stdlib.h>
#include <iostream>
#include <vector>

#include "Projectile.h"
#include "Button.h"
#include "Wave.h"
//#include "ScoreBoard.h"

using namespace sf;

typedef void (*changeLevel)(std::string);

#define PLAYER_SPEED 10

class Level
{
protected:
  int resW, resH;
  RenderWindow *window;
  Event *event;
  RectangleShape background;
  changeLevel cl;

public:
  Clock levelClock;

  Level()
  {
    resW = 800;
    resH = 600;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(Color(255,255,255)); // white
  }
  Level(RenderWindow &win, Event &ev, changeLevel cl)
  {
    // Get width of height of current winow
    resW = win.getSize().x;
    resH = win.getSize().y;
    background.setSize(Vector2f(resW,resH));
    background.setPosition(0,0);
    background.setFillColor(Color(255,255,255)); // white
    window = &win;
    event = &ev;
    this->cl = cl;
  }
  virtual ~Level()
  {

  }

  virtual void resize()
  {

  }

  void checkWindowSize()
  {
    resW = window->getView().getSize().x;
    resH = window->getView().getSize().y;
    background.setSize(Vector2f(resW,resH));
    //resize();
  }

  virtual void draw()
  {
    //All levels must be drawn
  }

};

#include "Player.h"
#include "Enemy.h"
class TestLevel : public Level
{
  Player p;
  Texture playerTexture;                // What the player looks like
  float ratio = (float) 2 / 3;          // Ratio gameplay:points/text 2/3
  bool waitingForNextLevel = true;      // Check whether we are waiting for next level
  bool gameIsOver = false;              // Track whether the game is over
  Font gameFont;                        // Font this level is using
  RectangleShape *boundingLine;         // Line at 2/3rds screen width

  // Keep track of player/enemy projectiles
  proj_map *playerProjectiles = new proj_map;
  proj_map *enemyProjectiles = new proj_map;
  int_vec  *deadProjectiles = new int_vec;
  enemy_map *enemies = new enemy_map;

  std::vector<Wave*> waves;             // Keep track of all possible waves
  int currWaveIndex = -1;               // Current wave that we're on
  int currScore = 0;                    // Keep track of current player score

  Text statusText;                      // Shows "Hit space to start!" text
  Text score;                           // Displays your score at the right
  Text combo;                           // Shows combos

public:
  TestLevel(RenderWindow &win, Event &ev, changeLevel cl,Font fin) : Level(win,ev,cl)
  {
    // Set up level characteristics
    gameFont = fin;
    FloatRect bbnd = background.getGlobalBounds();
    background.setFillColor(Color(28,64,93)); // dark blue

    // Create right, black boundary line
    boundingLine = new RectangleShape(Vector2f(0,bbnd.height));
    boundingLine->setPosition(bbnd.width*ratio,0);
    boundingLine->setOutlineColor(Color(0,0,0));
    boundingLine->setOutlineThickness(2);

    // Show score and starting text
    showText();

    // Create player
    playerTexture.loadFromFile("images/Skateboard_Forward.png");
    p = Player(playerTexture, playerProjectiles, enemyProjectiles, enemies, PLAYER_SPEED, FloatRect(bbnd.left,bbnd.top,bbnd.width*ratio,bbnd.height));

    // Set up waves
    waves.push_back(new WaveOne(playerProjectiles, enemyProjectiles, deadProjectiles, enemies));
    waves.push_back(new WaveTwo(playerProjectiles, enemyProjectiles, deadProjectiles, enemies));

    readyUpForNextWave();
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

    window->draw(*boundingLine);
    window->draw(statusText);
    window->draw(score);
    window->draw(combo);

    waves.at(currWaveIndex)->draw(*window);

    // TODO: Check that none of the player/enemy projectiles have gone off screen
    int_vec toBeDeleted;
    for (std::pair<int, Enemy*> enemyPair : *enemies)
    {
      enemyPair.second->draw(*window);

      // If enemy is dead, remove them from screen
      if (enemyPair.second->isDead())
      {
        // If collided, remove from hashmap
        toBeDeleted.push_back(enemyPair.first);

        // Increase player score
        currScore += enemyPair.second->score;

        std::string scoreString = "Score:\n" + std::to_string(currScore);
        score.setString(scoreString);
      }
    }

    // Delete all projectiles that went off-screen
    for (auto &enemyKey : toBeDeleted)
    {
      enemies->erase(enemyKey);
    }

    // Delete all dead projectiles
    for (auto &projKey: *deadProjectiles)
    {
      playerProjectiles->erase(projKey);
    }

    // Delete all dead projectiles
    deadProjectiles->clear();

    // Don't draw player if they're dead
    if (!p.isDead())
    {
      window->draw(p); //draw the player
      p.draw(*window); //let the player draw
    } else {
      gameOver();
    }

    // Check for end of wave
    if (!gameIsOver && !waitingForNextLevel && waves[currWaveIndex]->waveIsFinished())
    {
      readyUpForNextWave();
    }

    if (!gameIsOver && waitingForNextLevel && Keyboard::isKeyPressed(Keyboard::Space))
    {
      startWave();
      waitingForNextLevel = false;
    }
  }

  void showText()
  {
    statusText.setFont(gameFont);
    score.setFont(gameFont);
    combo.setFont(gameFont);

    statusText.setString("Press space to start!");
    score.setString("Score: \n0");
    combo.setString("Combo: \n1x");

    statusText.setCharacterSize(50);
    score.setCharacterSize(50);
    combo.setCharacterSize(50);

    statusText.setColor(Color::White);
    score.setColor(Color::White);
    combo.setColor(Color::White);

    FloatRect bbnd = background.getGlobalBounds();
    statusText.setPosition(Vector2f(bbnd.width/12, bbnd.height/2));
    score.setPosition(Vector2f(bbnd.width - 300, 50));
    combo.setPosition(Vector2f(score.getPosition().x, score.getPosition().y + 150));
  }

  void setCurrentWaveIndex (int wave)
  {
    currWaveIndex = wave;
  }

  int getCurrentWave ()
  {
    return currWaveIndex;
  }

  void readyUpForNextWave ()
  {
    waitingForNextLevel = true;
    statusText.setString("Press space to start!");

    if (currWaveIndex + 1 >= waves.size())
    {
      // Next wave not found
      // You won!
      std::cout << "No next wave found, you musta won!!!!" << std::endl;
      gameOver();
      statusText.setString("You won!");
    } else {
        // Go to next wave
        currWaveIndex++;
    }
  }

  void startWave ()
  {
    // Show wave text
    statusText.setString("Welcome to Wave " + std::to_string(currWaveIndex + 1));
    statusText.setPosition(Vector2f(background.getGlobalBounds().width/12, 
                                      50));

    // Find index of current wave
    // Start wave
    waves.at(currWaveIndex)->setBoundaries(Rect<int>(background.getGlobalBounds()));
    waves.at(currWaveIndex)->spawnEnemies();
  }

  // You lose
  void gameOver()
  {
    statusText.setPosition(Vector2f(background.getGlobalBounds().width/6, 
                                      50));
    statusText.setString("Game over!");
    gameIsOver = true;

    //writeNewScore(currScore);
  }
};

class PauseMenu : public Level
{
  Font gameFont;

  // Buttons
  Button *backButton;

public:
  int pLevelIndex;

  PauseMenu(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
  {
    gameFont = fin;

    // Display a back button
    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());
  }
  ~PauseMenu()
  {

  }

//--------------------------------IMPLEMENTATION-----------------------------------//
  void resize()
  {
    // Display a back button
    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());
  }

  void draw()
  {
    checkWindowSize();
    backButton->draw(*window);

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      if(backButton->contains(mouseX,mouseY))
        cl("main");
    }
    else
      backButton->checkHover(mouseX,mouseY);
  }

};


#endif
