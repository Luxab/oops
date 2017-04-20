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
  Weapon *weapon; //Player's current weapon
  Texture left, right, up, down;

  Player()
  {

  }
  Player(Texture &tin, int s, int h) : Sprite(tin)
  {
    speed = s;
    health = h;
    //setPosition(rin.top, rin.left);

    left.loadFromFile("images/leftsign.png");
    right.loadFromFile("images/rightsign.png");
    up.loadFromFile("images/upsign.png");
    down.loadFromFile("images/downsign.png");

    // Default weapon
    weapon = new BBGun();
  }
  ~Player()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = getPosition();
    w->shoot(pos);
  }

  void tickMove()
  {
    IntRect tRec = getTextureRect();
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
      move(-speed,0);
      setTexture(left);
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
      move(speed,0);
      setTexture(right);
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
      move(0,speed);
      setTexture(down);
    }
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
      move(0,-speed);
      setTexture(up);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
      shoot(weapon);
    }
    //Move in {direction} at {speed}
    //setPosition(x,y);

    // Apply changes
    setTextureRect(tRec);
  }

  void draw(RenderWindow &win)
  {
    tickMove();
    weapon->draw(win);

    // Draw the weapon
    //weapon.draw();
  }

  bool contains(int mx, int my)
  {
    return getTextureRect().contains(mx,my);
  }

  void setWeapon(Weapon *w)
  {
    weapon = w;
  }
};
