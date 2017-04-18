//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Weapon.h"

using namespace sf;

class Player : public Sprite
{
public:
  int health; //How much damage can it take
  float speed; //How fast does it move
  Weapon weapon; //Player's current weapon

  Player()
  {

  }
  Player(Texture &tin, IntRect &rin, int s, int h) : Sprite(tin,rin)
  {
    speed = s;
    health = h;
    setPosition(rin.top, rin.left);

    // Default weapon
    BBGun b;
    weapon = b;
  }
  ~Player()
  {

  }

  void shoot (Weapon w)
  {
    Vector2f pos = getPosition();
    w.shoot(pos);
  }

  void tickMove()
  {
    IntRect tRec = getTextureRect();
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
      move(-speed,0);
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
      move(speed,0);
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
      move(0,speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
      move(0,-speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {

    }
    //Move in {direction} at {speed}
    //setPosition(x,y);

    // Apply changes
    setTextureRect(tRec);
  }

  void draw()
  {
    tickMove();

    // Draw the weapon
    //weapon.draw();
  }

  bool contains(int mx, int my)
  {
    return getTextureRect().contains(mx,my);
  }

  void setWeapon(Weapon w)
  {
    weapon = w;
  }
};
