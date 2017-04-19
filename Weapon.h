//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace sf;

class Weapon
{
public:
  float speed; //How fast does it move
  float potency; //How much damage each shot does
  IntRect size; //Size of each shot
  Texture shotTexture; // What the shot looks like
  std::vector<Projectile> projectiles;

  Weapon()
  {

  }
  ~Weapon()
  {

  }

  void draw(RenderWindow &win)
  {
    std::cout << projectiles.size() << std::endl;
    for (int i = 0; i<projectiles.size(); i++)
    {
      win.draw(projectiles[i]);
    }
  }

  virtual void setWeaponRect() = 0;

  virtual void shoot(Vector2f initPos) = 0;
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

      setWeaponRect();
    }
    ~BBGun()
    {

    }

    virtual void setWeaponRect()
    {
      // Size of each shot
      size = IntRect(0, 0, 92, 92);
    }

    virtual void shoot(Vector2f initPos)
    {
      // Single shot up
      std::cout << "pew" << std::endl;
      projectiles.push_back(Projectile(initPos.x, initPos.y, shotTexture, size, speed, 0, potency));
   }
};
