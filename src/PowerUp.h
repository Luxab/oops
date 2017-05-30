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

#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include <math.h> // sin,cos
#include <random>
#include "definitions.h"

using namespace sf;

typedef enum POWERUP_TYPE
{
  WEAPON_POWERUP,
  HEALTH_POWERUP
} POWERUP_TYPE;

class PowerUp : public Sprite
{
public:
  Vector2f pos;
  float speed;
  float healthBoost = 0;            // How much health does the powerup provide
  Weapon *weapon;                   // What weapon this powerup gives you
  Texture powerupTexture;           // What the powerup looks like
  IntRect boundaries;               // Window bounds
  pow_map *powerups;                // Keep track of powerups that are spawned
  proj_map *playerProjectiles;      // Keep track of the player's projectiles

  Music grabSound;                  // Sound that plays when powerup is picked up

  PowerUp(IntRect b, pow_map *p, proj_map *pp)
  {
    this->playerProjectiles = pp;
    this->boundaries = b;
    this->powerups = p;
    this->pos = pos;

    // Set this in derived classes
    weapon = nullptr;
  }
  ~PowerUp()
  {

  }

  void tickMove()
  {
    //Move in {direction} at {speed}
    float moveX = 0;
    float moveY = 1;

    // Scale by the speed
    moveX *= speed;
    moveY *= speed;

    move(moveX, moveY);
  }

  void draw (RenderWindow &win)
  {
    Sprite toDraw = *this;
    win.draw(toDraw);

    tickMove();
  }

  bool contains(FloatRect rect)
  {
    return getGlobalBounds().intersects(rect);
  }

  void playSound ()
  {
    grabSound.play();
  }

  float getHealthBoost()
  {
    return healthBoost;
  }

  Weapon* getWeapon()
  {
    return weapon;
  }
};

class SnackBar : public PowerUp
{
    public:
      SnackBar (IntRect b, pow_map *pin, proj_map *ppin) : PowerUp (b,pin,ppin)
      {
        std::cout << "Creating new snackbar." << std::endl;

        // Set characteristics
        healthBoost = 2;
        speed = 1;

        powerupTexture.loadFromFile("images/snackbar.png");
        grabSound.openFromFile("audio/item_pickup.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.5,0.5));
      }

      ~SnackBar ()
      {

      }
};

class Doritos : public PowerUp
{
    public:
      Doritos (IntRect b, pow_map *pin, proj_map *ppin) : PowerUp (b,pin,ppin)
      {
        std::cout << "Creating new doritos." << std::endl;

        // Set characteristics
        healthBoost = 3;
        speed = 1;

        powerupTexture.loadFromFile("images/doritos.png");
        grabSound.openFromFile("audio/item_pickup.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.3,0.3));
      }

      ~Doritos ()
      {

      }
};

class AK47PowerUp : public PowerUp
{
    public:
      AK47PowerUp (IntRect b, pow_map *pin, proj_map *ppin) : PowerUp (b,pin,ppin)
      {
        std::cout << "Creating new ak47." << std::endl;

        // Set characteristics
        weapon = new AK47(boundaries, playerProjectiles);
        healthBoost = 0;
        speed = 1;

        powerupTexture.loadFromFile("images/ak47.png");
        grabSound.openFromFile("audio/weapon_sounds/ak47_reload.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.6,0.6));
      }

      ~AK47PowerUp()
      {

      }
};

class SpreadEaglePowerUp : public PowerUp
{
    public:
      SpreadEaglePowerUp (IntRect b, pow_map *pin, proj_map *ppin) : PowerUp (b,pin,ppin)
      {
        std::cout << "Creating new spread eagle power up." << std::endl;

        // Set characteristics
        weapon = new SpreadEagle(boundaries, playerProjectiles);
        healthBoost = 0;
        speed = 1;

        powerupTexture.loadFromFile("images/spread_eagle.png");
        grabSound.openFromFile("audio/weapon_sounds/ak47_reload.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.1,0.1));
      }

      ~SpreadEaglePowerUp()
      {

      }
};

#endif
