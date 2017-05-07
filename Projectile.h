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

#ifndef PROJECTILE_H
#define PROJECTILE_H

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
    this->rotate(moveDir);

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

  float getPotency ()
  {
    return potency;
  }
};

#endif
