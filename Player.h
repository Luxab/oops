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

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "HealthBar.h"
#include <vector>
#include <iostream>
#include <string>

using namespace sf;

class Player : public Sprite
{
public:
  HealthBar *health;
  //int health;                         // How much damage can it take
  float speed;                          // How fast does it move
  Weapon *weapon;                       // Player's current weapon
  IntRect boundaries;                   // Rectangle that defines level boundaries
  float rightSide;
  float bottomSide;
  Texture left, right, up, down, transparent;
  Clock weaponCooldown;
  Clock immunityClock;                  // Keep track of how long invincibility lasts
  double immunitySeconds = 1000;        // Milliseconds player gets immunity after damage
  bool currentlyImmune = false;         // Keep track of when we are immune
  bool currentlyTransparent = false;    // Keep track of when we are immune
  bool immunityFirstCheck = false;      // There are too many bools here
  proj_map *enemyProjectiles;           // Keep track of all enemy projectiles on screen
  proj_map *playerProjectiles;          // Keep track of all projectiles we fire
  enemy_map *enemies;                   // Keep track of all enemies
  pow_map *powerups;                    // Keep track of all powerups
  bool dead = false;

  Sound *painSound = new Sound;
  SoundBuffer *painBuffer = new SoundBuffer;

  Sound *deathSound = new Sound;              // Sound that plays when weapon is shot
  SoundBuffer *deathSoundBuffer = new SoundBuffer;  // Buffer for shot sound

  Player()
  {

  }
  Player(Texture &tin, proj_map *pp, proj_map *ep, enemy_map *e, pow_map *p, int s, FloatRect b) : Sprite(tin)
  {
    painSound->setBuffer(*painBuffer);

    deathSoundBuffer->loadFromFile("audio/death_yell.wav");
    deathSound->setBuffer(*deathSoundBuffer);

    // Projectile tracking
    playerProjectiles = pp;
    enemyProjectiles = ep;
    enemies = e;
    powerups = p;

    // Player characteristics
    speed = s;
    boundaries = Rect<int> (b);
    rightSide = boundaries.left+boundaries.width;
    bottomSide = boundaries.top+boundaries.height;

    float ratio = .5;
    Vector2f barLoc(b.left+b.width+2,2);
    Vector2f barSize(b.width*ratio,b.height*.05);
    health = new HealthBar(barLoc,barSize,5);

    left.loadFromFile("images/Skateboard_Left.png");
    right.loadFromFile("images/Skateboard_Right.png");
    up.loadFromFile("images/Skateboard_Forward.png");
    down.loadFromFile("images/Skateboard_Forward.png");
    transparent.loadFromFile("images/transparent.png");

    // Default weapon
    weapon = new BBGun(boundaries, playerProjectiles);

    // Set position to middle
    setPosition(boundaries.width/2, boundaries.height/2);
    setScale(2,2);
  }
  ~Player()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = getPosition();

    // Only fire if timer is past cooldown
    if (weaponCooldown.getElapsedTime().asMilliseconds() > w->cooldown)
    {
        w->shoot(pos);
        weaponCooldown.restart();
    }
  }

  void tickMove()
  {
    IntRect tRec = getTextureRect();

    bool lMov =  Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::Left);
    bool rMov = Keyboard::isKeyPressed(Keyboard::D) ||
        Keyboard::isKeyPressed(Keyboard::Right);
    bool dMov = Keyboard::isKeyPressed(Keyboard::S) ||
        Keyboard::isKeyPressed(Keyboard::Down);
    bool uMov = Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::Up);

    float tSpeed = speed;
    if(lMov+rMov+dMov+uMov>1)
      tSpeed*=.5;

    if (lMov)
    {
      movePlayer(-tSpeed,0);
      setTexture(left);
    }
    if (rMov)
    {
      movePlayer(tSpeed,0);
      setTexture(right);
    }
    if (dMov)
    {
      movePlayer(0,tSpeed);
      setTexture(down);
    }
    if (uMov)
    {
      movePlayer(0,-tSpeed);
      setTexture(up);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
      shoot(weapon);
    }
    //Move in {direction} at {speed}
    //setPosition(x,y);

    // Set correct texture based on immunity value
    if (currentlyTransparent)
      this->setColor(Color(255,255,255,100));
    else
      this->setColor(Color(255,255,255,255));

    // Apply changes
    setTextureRect(tRec);
  }

  // Move player and check boundaries
  // Returns true if move was successful, false if otherwise
  bool movePlayer (float x, float y)
  {
    FloatRect bounds = getGlobalBounds();

    // Ensure player hasn't gone out of bounds
    if (bounds.left+x < boundaries.left)
    {
      move(boundaries.left,0);
    }
    else if (bounds.top+y<boundaries.top)
    {
      move(0,boundaries.top);
    }
    else if (bounds.left+bounds.width+x > rightSide)
    {
      move(rightSide-bounds.left-bounds.width,0);
    }
    else if (bounds.top+bounds.height+y > bottomSide)
    {
      move(0,bottomSide-bounds.top-bounds.height);
    }
    else
    {
      // If not hitting a wall, move
      move (x,y);

      return true;
    }

    return false;
  }

  // Check if we're intersecting enemies or their
  // projectiles
  void checkProjectiles (RenderWindow &win)
  {
    for (std::pair<int, Projectile*> shotPair : *enemyProjectiles)
    {
      // Workaround to appease vtable gods
      Projectile *shot = shotPair.second;
      if (shot->contains(getGlobalBounds()))
      {
        loseHealth(win,shot->getPotency());
      }
    }

    for (std::pair<int, Enemy*> enemyPair : *enemies)
    {
      Enemy *enemy = enemyPair.second;
      if (enemy->contains(getGlobalBounds()))
      {
        loseHealth(win,1);
      }
    }
  }

  void checkPowerUps (RenderWindow &win)
  {
    std::vector<int> powerupsToBeDeleted;

    for (std::pair<int, PowerUp*> powerupPair : *powerups)
    {
      PowerUp *powerup = powerupPair.second;
      if (powerup->contains(getGlobalBounds()))
      {
        std::cout << "Got powerup, adding health: " << powerup->getHealthBoost() << std::endl;
        // If we're intersecting, get the powerup's benefits
        if (powerup->getHealthBoost() != 0)
        {
          health->addHealth(powerup->getHealthBoost());
          health->draw(win);
        }
        
        if (powerup->getWeapon())
          weapon = powerup->getWeapon();

        // Play the powerup pickup sound
        powerup->playSound();

        powerupsToBeDeleted.push_back(powerupPair.first);
      }
    }

    for (int powerupKey : powerupsToBeDeleted)
    {
      powerups->erase(powerupKey);
    }
  }

  void draw(RenderWindow &win)
  {
    // Check if we've run into any enemy projectiles
    if (!currentlyImmune)
    {
        checkProjectiles(win);
        currentlyTransparent = false;
    }

    checkPowerUps(win);
    animateImmunity();

    tickMove();
    weapon->draw(win);
    health->draw(win);
  }

  bool contains(FloatRect rect)
  {
    return getGlobalBounds().intersects(rect);
  }

  void animateImmunity ()
  {
    // Check if we just took damage, and if so be transparent
    if (immunityClock.getElapsedTime().asMilliseconds() < immunitySeconds)
    {
      if (immunityFirstCheck)
      {
        currentlyImmune = true;

        // Animate between transparent and non-transparent based on clock
        if (immunityClock.getElapsedTime().asMilliseconds() % 100 >= 50)
          currentlyTransparent = true;
        else if (immunityClock.getElapsedTime().asMilliseconds() % 100 < 50)
          currentlyTransparent = false;
      }
    }
    else
    {
      currentlyImmune = false;
      immunityFirstCheck = true;
    }
  }

  void playPainSound()
  {
    std::string painString;
    painString+="audio/pain/man_pain_";
    int randNum = rand() % 29 + 1; //Range of 1 to 29
    painString+= std::to_string(randNum);
    painString+=".wav";
    painBuffer->loadFromFile(painString);
    painSound->play();
  }

  void loseHealth(RenderWindow &win, int amt)
  {
    playPainSound();
    health->takeDamage(amt);
    if (health->getCurrentHealth() <= 0)
        killSelf();

    // Start the immunity countdown
    immunityClock.restart();
  }

  void restartImmunityClock ()
  {
    immunityClock.restart();
  }

  bool isDead()
  {
    return dead;
  }

  void killSelf()
  {
    deathSound->play();

    dead = true;
    std::cout << "You ded!" << std::endl;
  }

  void setWeapon(Weapon *w)
  {
    weapon = w;
  }
};

#endif
