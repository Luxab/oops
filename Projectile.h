//  Created by Duncan Klug on 4/11/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Projectile : Sprite
{
public:
  int potency; //How much damage does the projectile do
  int speed; //How fast does the projectile move
  //TODO: Implement direction the projectile will move in
  //Position will be measured in floats

  Projectile(int xin, int yin, Texture &tin, IntRect &rin) : Sprite(tin,rin)
  {
    setPosition(xin,yin);
  }
  ~Projectile()
  {

  }

  void tickMove()
  {
    //Move in {direction} at {speed}
    //setPosition(x,y);
  }

  bool contains(int mx, int my)
  {
    return getTextureRect().contains(mx,my);
  }
};
