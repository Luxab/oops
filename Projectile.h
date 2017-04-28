#ifndef PROJECTILE_H
#define PROJECTILE_H

//  Created by Duncan Klug on 4/11/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include <math.h> // sin,cos
#include "definitions.h"

using namespace sf;

class Projectile : public Sprite
{
public:
  float potency; //How much damage does the projectile do
  float speed; //How fast does the projectile move
  float moveDirection; // In what direction the projectile will move
  float posX, posY;

  Projectile(int xin, int yin, Texture &tin, float speed, float moveDir, float potency) : Sprite(tin)
  {
    // Set characteristics
    this->posX = xin;
    this->posY = yin;
    this->speed = speed;
    this->potency = potency;
    this->moveDirection = moveDir;

    // Set initial position
    setPosition(xin,yin);
  }
  ~Projectile()
  {

  }

  void tickMove()
  {
    //Move in {direction} at {speed}
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

#endif
