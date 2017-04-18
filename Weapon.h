//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Projectile.h"

using namespace sf;

class Weapon
{
public:
  float speed; //How fast does it move
  float potency; //How much damage each shot does
  IntRect size; //Size of each shot
  Texture shotTexture; // What the shot looks like

  Weapon()
  {
    setWeaponRect();
  }
  ~Weapon()
  {
    
  }

  virtual void setWeaponRect()
  {
    /* Implement size of Weapon shot */
  }

  virtual void shoot(Vector2f initPos)
  {
    /* Implement projectile shooting */
  }
};

class BBGun : public Weapon
{
  public:
    BBGun()
    {
      // Set BB characteristics
      potency = 1;
      speed = 0.5;
      shotTexture.loadFromFile("images/bb.jpg");
    }
    ~BBGun()
    {

    }

    virtual void setWeaponRect()
    {
      // Size of each shot 
      size = IntRect(0, 192, 192, 192);
    }

    virtual void shoot(Vector2f initPos)
    {
      // Single shot up
      Projectile(initPos.x, initPos.y, shotTexture, size, speed, 0, potency);
   }
};
