#ifndef POWERUP_H 
#define POWERUP_H

//  Created by Duncan Klug on 4/11/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include <math.h> // sin,cos
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
  float posX, posY;
  float speed;
  Texture powerupTexture;

  PowerUp()
  {
  }
  ~PowerUp()
  {

  }

  void tickMove()
  {
    //Move in {direction} at {speed}
    double moveDirection = 0;
    float moveX = sin(moveDirection);
    float moveY = cos(moveDirection);

    // Scale by the speed
    moveX *= speed;
    moveY *= speed;

    // Add to our existing position
    this->posX += moveX;
    this->posY -= moveY;

    setPosition(posX, posY);
  }

  bool contains(FloatRect rect)
  {
    return getGlobalBounds().intersects(rect);
  }
};

/*
class SnackBar : public PowerUp
{
    public:
      SnackBar (pow_map *pin) : PowerUp (p)
      {
        powerupTexture.loadFromFile("images/transparent.png");
      }
    
};
*/

#endif
