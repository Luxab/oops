//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace sf;

class Weapon
{
public:
  int shotCount; // Incrememnts per shot
  float speed; //How fast does it move
  float potency; //How much damage each shot does
  IntRect size; //Size of each shot
  Texture shotTexture; // What the shot looks like
  std::unordered_map<int, Projectile*> projectiles;
  IntRect boundingRect; // If projectiles leave this rect, delete them

  Weapon()
  {

  }
  ~Weapon()
  {

  }

  void draw(RenderWindow &win)
  {
    std::vector<int> toBeDeleted;
    for (auto &shot : projectiles)
    {
      win.draw(*shot.second);
      shot.second->tickMove();

      // Ensure bullet hasn't gone out of bounds
      if (!boundingRect.intersects(Rect<int>(shot.second->getGlobalBounds())))
      {
        // Has gone out of bounds, remove from hashmap
        toBeDeleted.push_back(shot.first);
      }
    }

    // Delete all projectiles that went off-screen
    for (auto &shotKey : toBeDeleted)
    {
      projectiles.erase(shotKey);
    }
  }

  // virtual void setWeaponRect() = 0;

  virtual void shoot(Vector2f initPos) = 0;
};

class BBGun : public Weapon
{
  public:
    BBGun()
    {
      // Set characteristics
      potency = 1;
      speed = 20;
      shotTexture.loadFromFile("images/bb.png");
    }
    ~BBGun()
    {

    }

    virtual void shoot(Vector2f initPos)
    {
      // Single shot up
      std::cout << "pew" << std::endl;

      // Insert new projectile into projectiles map 
      Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
      std::pair<int,Projectile*> newShot (shotCount++, proj);
      projectiles.insert(newShot);
   }
};

class SpreadEagle : public Weapon
{
  public:
    SpreadEagle()
    {
      // Set characteristics
      potency = 1;
      speed = 20;
      shotTexture.loadFromFile("images/bb.png");
    }
    ~SpreadEagle()
    {

    }

    virtual void shoot(Vector2f initPos)
    {
      // Three shots, one up and two to up and side

      // Insert new projectile into projectiles map 
      Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
      std::pair<int,Projectile*> newShot (shotCount++, proj);
      projectiles.insert(newShot);
   }
};
