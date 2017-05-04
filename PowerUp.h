#ifndef POWERUP_H 
#define POWERUP_H

//  Created by Duncan Klug on 4/11/17.
//	Lab 02 Spring 2017
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
        healthBoost = 2;
        speed = 1;

        powerupTexture.loadFromFile("images/bb.png");
        grabSoundBuffer.loadFromFile("audio/shot_sound.wav");

        // Set texture
        setTexture(powerupTexture);

        // Set size of the powerup
        setScale(Vector2f(1.2,1.2));
      }

      ~SnackBar ()
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
        weapon = new SpreadEagle(boundaries, playerProjectiles);
        healthBoost = 0;
        speed = 1;

        powerupTexture.loadFromFile("images/ak47.png");
        grabSoundBuffer.loadFromFile("audio/shot_sound.wav");

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
