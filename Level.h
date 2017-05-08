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

#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Projectile.h"
#include "Button.h"
#include "Wave.h"

#define MAX_WAVES 26

using namespace sf;

typedef void (*changeLevel)(std::string);
typedef void (*startNewGame)();

// Global
int currScore = 0;                    // Keep track of current player score
std::vector<Wave*> waves;             // Keep track of all possible waves
int currWaveIndex = -1;               // Current wave that we're on

#define PLAYER_SPEED 10

class Level
{
protected:
  int resW, resH;
  RenderWindow *window;
  Event *event;
  RectangleShape background;
  changeLevel cl;
  startNewGame sng;

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

  virtual void didAppear()
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

class PauseMenu : public Level
{
  Font gameFont;

  // Buttons
  Button *backButton;
  Button *pauseNotif;

public:
  int pLevelIndex;

  PauseMenu(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
  {
    gameFont = fin;

    // Display a back button
    backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
    backButton->setY(resH-backButton->getHeight());
    pauseNotif = new Button(0,resH/5,gameFont,"Game Paused",.06*resW);
    pauseNotif->centerWidth(resW);
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
    pauseNotif->draw(*window);

    //--------------Mouse Input--------------//
    Vector2i mousepos = Mouse::getPosition(*window);
    float mouseX = mousepos.x;
    float mouseY = mousepos.y;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
      if(backButton->contains(mouseX,mouseY))
      {
        // Stop level music
        waves.at(currWaveIndex)->stopWaveMusic();

        currWaveIndex = -1;
        cl("main");
      }
    }
    else
      backButton->checkHover(mouseX,mouseY);
  }
};

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
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
  pow_map *powerups = new pow_map;

  Text statusText;                      // Shows "Hit space to start!" text
  Text combo;                           // Shows combos
  Text score;                           // Displays your score at the right

  IntRect boundaries;                   // Boundaries of the level
  Clock welcomeTextTimerClock;          // Timer for making welcome text disappear
  bool welcomeTextShowing = false;      // Is the welcome text showing?
  double welcomeTextShownLength = 3000; // How long the welcome text fades out for

public:
  TestLevel(RenderWindow &win, Event &ev, changeLevel cl,Font fin) : Level(win,ev,cl)
  {
    // Set up level characteristics
    gameFont = fin;
    boundaries = Rect<int>(background.getGlobalBounds());
    background.setFillColor(Color(28,64,93)); // dark blue

    // Create right, black boundary line
    boundingLine = new RectangleShape(Vector2f(0,boundaries.height));
    boundingLine->setPosition(boundaries.width*ratio,0);
    boundingLine->setOutlineColor(Color(0,0,0));
    boundingLine->setOutlineThickness(2);


    // Show score and starting text
    showText();

    // Create player
    playerTexture.loadFromFile("images/Skateboard_Forward.png");
    p = Player(playerTexture, playerProjectiles, enemyProjectiles, enemies, powerups, PLAYER_SPEED, FloatRect(boundaries.left,boundaries.top,boundaries.width*ratio,boundaries.height));
    p.restartImmunityClock();

    // Set up waves
    waves.push_back(new WaveOne(boundaries, playerProjectiles, enemyProjectiles, deadProjectiles, enemies, powerups));
    waves.push_back(new WaveTwo(boundaries, playerProjectiles, enemyProjectiles, deadProjectiles, enemies, powerups));
    waves.push_back(new WaveThree(boundaries, playerProjectiles, enemyProjectiles, deadProjectiles, enemies, powerups));

    float difficulty = 25;
    /*for(int w = 0; w < MAX_WAVES; w++) {
      waves.push_back(new WaveProcedural(boundaries, playerProjectiles, enemyProjectiles, deadProjectiles, enemies, powerups, difficulty));
      difficulty *= 2;
    }*/

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

    // Fade out welcome to wave X text
    if (!gameIsOver && welcomeTextShowing && welcomeTextTimerClock.getElapsedTime().asMilliseconds() > welcomeTextShownLength)
    {
      // Fade alpha slightly every frame
      statusText.setColor(Color(255,255,255,statusText.getColor().a - 2));

      // Once text has been faded out, remove it
      if (statusText.getColor().a <= 5)
      {
        // Kill alpha
        statusText.setColor(Color(255,255,255,0));

        // Stop timer
        welcomeTextShowing = false;
      }
    }

    // If enemy is dead or off screen, remove them
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
      // Check if enemy has gone out of bounds
      else if (!boundaries.intersects(Rect<int>(enemyPair.second->getGlobalBounds())))
      {
        // Has gone out of bounds, remove from hashmap
        toBeDeleted.push_back(enemyPair.first);
      }
    }

    // Delete all dead or off-screen enemies
    for (auto &enemyKey : toBeDeleted)
    {
      enemies->erase(enemyKey);
    }

    // Delete all dead player projectiles
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
    } else if (!gameIsOver){
      gameOver();
    }

    // Draw all powerups and remove if gone out of bounds
    std::vector<int> powerupsToBeDeleted;

    for (std::pair<int, PowerUp*> pow : *powerups)
    {
      // Workaround to appease vtable gods
      PowerUp* powObj = pow.second;
      Sprite toDraw = *powObj;
      window->draw(toDraw);
      powObj->draw(*window);

      pow.second->tickMove();

      // Ensure powerup hasn't gone out of bounds
      if (!boundaries.intersects(Rect<int>(pow.second->getGlobalBounds())))
      {
        // Has gone out of bounds, remove from hashmap
        powerupsToBeDeleted.push_back(pow.first);
      }

    }

    // Delete all powerups that went off-screen
    for (auto &powKey : powerupsToBeDeleted)
    {
      powerups->erase(powKey);
    }

    // Check for end of wave
    if (!gameIsOver && !waitingForNextLevel && waves[currWaveIndex]->waveIsFinished())
    {
      waves[currWaveIndex]->cleanup();
      readyUpForNextWave();
    }

    if (!gameIsOver && waitingForNextLevel && Keyboard::isKeyPressed(Keyboard::Space))
    {
      startWave();
      waitingForNextLevel = false;
    }

    // Easter egg
    if (!gameIsOver && Keyboard::isKeyPressed(Keyboard::P))
    {
      p.setWeapon(new Cannon(boundaries,playerProjectiles));
    }


    // Wait a bit after dying
    if (gameIsOver && Keyboard::isKeyPressed(Keyboard::Return))
    {
      waves[currWaveIndex]->cleanup();
      currWaveIndex = -1;
      cl("victory");
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
    waves[currWaveIndex]->cleanup();
    if (currWaveIndex + 1 >= waves.size())
    {
      // Next wave not found
      // You won!
      std::cout << "No next wave found, you musta won!!!!" << std::endl;
      gameOver();
      statusText.setString("      You won!\n\nEnter to Continue");
    statusText.setColor(Color(10,255,10)); // Set to green
    } else {
      // Go to next wave
      currWaveIndex++;
    }
  }

  void startWave ()
  {
    // Show wave text
    statusText.setString("Welcome to Wave " + std::to_string(currWaveIndex + 1));
    statusText.setPosition(Vector2f(background.getGlobalBounds().width/12, 50));
    statusText.setColor(Color(255,255,255)); // Set to white

    std::cout << "Starting wave!" << std::endl;
    welcomeTextShowing = true;

    // Find index of current wave
    // Start wave
    waves.at(currWaveIndex)->setBoundaries(Rect<int>(background.getGlobalBounds()));
    waves.at(currWaveIndex)->loadEnemiesAndPowerups();
    waves.at(currWaveIndex)->spawnEnemies();
  }

  // You lose
  void gameOver()
  {
    waves[currWaveIndex]->cleanup();
    statusText.setPosition(Vector2f(background.getGlobalBounds().width/6, 50));
    statusText.setString("Game over!\n\nPress Enter");
    statusText.setColor(Color(255,10,10)); // Set to red
    gameIsOver = true;
  }
};

//---------------------------------------------------------------------------------------------------//

class VictoryScreen : public Level
{
  Font gameFont;
  Text enterInitialsText;

  // Buttons
  Button *backButton;
  Button *nameButton;
  Time holdUp;
  bool enoughIsEnough;
  std::string nameString = "";   // Name of the player, used for high scores

  bool inputAllowed = 0;
  bool initialDraw = 1;

  public:
    int pLevelIndex;

    VictoryScreen(RenderWindow &win, Event &ev, changeLevel cl, Font fin) : Level(win,ev,cl)
    {
      gameFont = fin;

      // "Enter your initials" text
      enterInitialsText.setFont(gameFont);
      enterInitialsText.setString("Enter your initials");
      enterInitialsText.setCharacterSize(60);
      enterInitialsText.setColor(Color::White);

      FloatRect bbnd = background.getGlobalBounds();
      enterInitialsText.setPosition(Vector2f(bbnd.width/4, 20));

      // Display a back button
      backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
      backButton->setY(resH-backButton->getHeight());

      nameButton = new Button(0,resH/4,gameFont,"_ _ _",(.06*resW));
      nameButton->centerWidth(resW);
      nameButton->setColor(Color(0,0,0,0));

      holdUp = levelClock.getElapsedTime();
    }
    ~VictoryScreen()
    {

    }

    void resize()
    {
      // Display a back button
      backButton = new Button(0,resH,gameFont,"Back",(.06*resW));
      backButton->setY(resH-backButton->getHeight());
    }

    void draw()
    {
      if (initialDraw)
      {
        holdUp = levelClock.getElapsedTime();
        initialDraw = 0;
      }
      checkWindowSize();
      backButton->draw(*window);
      window->draw(enterInitialsText);

      //Wait before allowing ANY input
      if(levelClock.getElapsedTime().asSeconds()-holdUp.asSeconds()>1)
      {
        holdUp = levelClock.getElapsedTime();
        inputAllowed = true;
      }

      if (inputAllowed)
      {
        if(Keyboard::isKeyPressed(Keyboard::Return) && nameString.size()>0)
        {
          writeNewScore(currScore);
          cl("scores");
        }
        //wait before allowing another input.
        if(levelClock.getElapsedTime().asSeconds()-holdUp.asSeconds()>.2)
        {
          holdUp = levelClock.getElapsedTime();
          enoughIsEnough = true;
        }

        if(enoughIsEnough)
        {
          if(Keyboard::isKeyPressed(Keyboard::BackSpace) && nameString.size()!=0)
          {
            nameString.pop_back();
            enoughIsEnough = false;
          }
          else if (nameString.size()<3)
          {
            if (nameString.size() == 0)
            {
              nameButton->setText("_ _ _");
              nameButton->centerWidth(resW);
            }

            char c = 65;
            //Iterates through the 'key' enumerator from A to Z, finds the appropriate key input
            for (int keyLoop = Keyboard::A; keyLoop != Keyboard::Num0; keyLoop++)
            {
              if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(keyLoop)))
              {
                nameString.push_back(c);
                enoughIsEnough = false;
                break;
              }
              c++;
            }
          }
          if(!enoughIsEnough)
          {
            nameButton->setText(nameString);
            nameButton->centerWidth(resW);
          }
        }
      }

      nameButton->draw(*window);

      //--------------Mouse Input--------------//
      Vector2i mousepos = Mouse::getPosition(*window);
      float mouseX = mousepos.x;
      float mouseY = mousepos.y;
      if (Mouse::isButtonPressed(Mouse::Left))
      {
        if(backButton->contains(mouseX,mouseY))
        {
          cl("main");
        }
      }
      else
        backButton->checkHover(mouseX,mouseY);
    }

    void writeNewScore (int newScore)
    {
      std::string scoreFileName = "scores.txt";
      std::ifstream textScores(scoreFileName);
      std::vector<std::string> nameVec;   // Store current names
      int_vec scoreVec;                   // Store current scores

      if (textScores)
      {
        // Read through score lines
        while (!textScores.eof())
        {
          std::string str, scoreStr, name;
          std::getline(textScores,str);

          // Get score from current score lines
          //

          std::istringstream nameiss(str);
          std::istringstream scoreiss(str);

          // Store name
          for (int i = 0; nameiss >> name; i++)
          {
            if (i == 1)
              break; // We got our name
          }

          // Store score
          for (int i = 0; scoreiss >> scoreStr; i++)
          {
            if (i == 2)
              break; // We got our score
          }

          // Convert score to int and insert it and
          // corresponding name into vector
          if (name != "" && scoreStr != "")
          {
            scoreVec.push_back(atoi(scoreStr.c_str()));
            nameVec.push_back(name);
          }
        }
      }

      // Insert score into vector at proper position
      std::vector<int>::iterator scoreIt = std::lower_bound(scoreVec.begin(), scoreVec.end(), currScore, std::greater<int>());
      std::vector<std::string>::iterator nameIt = nameVec.begin() + (scoreIt - scoreVec.begin());
      scoreVec.insert(scoreIt, currScore);
      nameVec.insert(nameIt, nameString);

      // If high score list is comprised of 10 or more people, cut off last person
      // it doesn't grow
      if (scoreVec.size() >= 10)
      {
        scoreVec.pop_back();
      }

      // Remove old list of scores
      if (textScores)
        remove(scoreFileName.c_str());

      // Write new list of scores
      std::ofstream outTextScores(scoreFileName);
      for (int i = 0; i < scoreVec.size();i++)
      {
        // Remove all zeros from score list
        if (scoreVec.at(i) != 0)
            outTextScores << i+1 << ": " << nameVec.at(i) << " " << scoreVec.at(i) << std::endl;
      }

      // Release file descriptor
      outTextScores.close();
    }
};

#endif
