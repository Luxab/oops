//  Created by Duncan Klug on 4/11/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include <math.h> // sin,cos

using namespace sf;

class Projectile : Sprite
{
public:
  float potency; //How much damage does the projectile do
  float speed; //How fast does the projectile move
  float moveDirection; // In what direction the projectile will move
  float posX, posY;

  Projectile(int xin, int yin, Texture &tin, IntRect &rin, float speed, float moveDir, float potency) : Sprite(tin,rin)
  {
    // Set initial position
    setPosition(xin,yin);

    // Set characteristics
    this->posX = xin;
    this->posY = yin;
    this->speed = speed;
    this->potency = potency;
    this->moveDirection = moveDir;
  }
  ~Projectile()
  {

  }

  void tickMove()
  {
    //Move in {direction} at {speed}
    //setPosition(x,y);
    float moveX = cos(moveDirection);
    float moveY = sin(moveDirection);

    // Scale by the speed
    moveX *= speed;
    moveY *= speed;

    // Add to our existing position
    this->posX += moveX;
    this->posY += moveY;

    setPosition(posX, posY);
  }

  bool contains(int mx, int my)
  {
    return getTextureRect().contains(mx,my);
  }
};
