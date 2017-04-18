//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Player : Sprite
{
public:
  int health; //How much damage can it take
  int speed; //How fast does it move

  Player(int xin, int yin, Texture &tin, IntRect &rin, int s, int h) : Sprite(tin,rin)
  {
    speed = s;
    health = h;
    setPosition(xin,yin);
  }
  ~Player()
  {

  }

  void tickMove()
  {
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
      setPosition(x-speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
      setPosition(x+speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
      setPosition(y+speed);
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
      setPosition(y-speed);
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
};
