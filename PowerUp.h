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
  float healthBoost;                // How much health does the powerup provide
  Weapon *weapon;                   // What weapon this powerup gives you
  Texture powerupTexture;           // What the powerup looks like
  IntRect boundaries;               // Window bounds
  pow_map *powerups;                // Keep track of powerups that are spawned
  proj_map *playerProjectiles;      // Keep track of the player's projectiles

  Sound grabSound;                  // Sound that plays when powerup is picked up
  SoundBuffer grabSoundBuffer;      // Buffer for grab sound

  PowerUp(IntRect b, pow_map *p, proj_map *pp)
  {
    this->playerProjectiles = pp;
    this->boundaries = b;
    this->powerups = p;
    this->pos = pos;
    
    grabSound.setBuffer(grabSoundBuffer);

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
        healthBoost = 1;
        speed = 1;

        powerupTexture.loadFromFile("images/bb.png");
        grabSoundBuffer.loadFromFile("audio/item_pickup.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(1.2,1.2));
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
        healthBoost = 2;
        speed = 1;

        powerupTexture.loadFromFile("images/doritos.png");
        grabSoundBuffer.loadFromFile("audio/item_pickup.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.3,0.3));
      }

      ~Doritos ()
      {

      }
};

class GunPowerUp : public PowerUp
{
    public:
      GunPowerUp (IntRect b, pow_map *pin, proj_map *ppin) : PowerUp (b,pin,ppin)
      {
        std::cout << "Creating new gun." << std::endl;

        // Set characteristics
        weapon = new AK47(boundaries, playerProjectiles);
        healthBoost = 0;
        speed = 1;

        powerupTexture.loadFromFile("images/ak47.png");
        grabSoundBuffer.loadFromFile("audio/weapon_sounds/shotgun_reload.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(0.6,0.6));
      }

      ~GunPowerUp()
      {

      }
};

#endif
