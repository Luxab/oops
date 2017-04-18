//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Weapon.h"

using namespace sf;

class Player : Sprite
{
public:
  int health; //How much damage can it take
  float speed; //How fast does it move
  Weapon weapon; //Player's current weapon

  Player(Texture &tin, IntRect &rin, int s, int h) : Sprite(tin,rin)
  {
    speed = s;
    health = h;
    setPosition(xin,yin);

    // Default weapon
    weapon = BBGun;
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
      setPosition(tRec.left-speed,tRec.top);
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
      setPosition(tRec.left+speed,tRec.top);
    }
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
      setPosition(tRec.left,tRec.top+speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
      setPosition(tRec.left,tRec.top-speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {

    }
    //Move in {direction} at {speed}
    //setPosition(x,y);
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
