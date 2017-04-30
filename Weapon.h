//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>

using namespace sf;

class Weapon
{
public:
  float speed;                  // How fast the shot travels
  float potency;                // How much damage each shot does
  float cooldown;               // How much time between each shot (milliseconds)
  IntRect size;                 // Size of each shot
  Texture shotTexture;          // What the shot looks like
  IntRect boundingRect;         // If projectiles leave this rect, delete them
  proj_map *projectiles;        // Keeps track of all projectiles on screen

  Weapon(IntRect b, proj_map *p)
  {
    boundingRect = b;
    projectiles = p;
  }
  ~Weapon()
  {

  }

  void draw(RenderWindow &win)
  {
    std::vector<int> toBeDeleted;
    for (std::pair<int, Projectile*> shot : *projectiles)
    {
      // Workaround to appease vtable gods
      Projectile * shotObj = shot.second;
      Sprite toDraw = *shotObj;
      win.draw(toDraw);

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
      projectiles->erase(shotKey);
    }
  }

  // virtual void setWeaponRect() = 0;

  virtual void shoot(Vector2f initPos) = 0;
};

// -------------------- Player Weaponry -------------------- //

class BBGun : public Weapon
{
  public:
    BBGun(IntRect b, proj_map *p) : Weapon (b, p)
    {
      // Set characteristics
      potency = 1;
      speed = 20;
      cooldown = 300;
      shotTexture.loadFromFile("images/bullet.png");
    }
    ~BBGun()
    {

    }

    virtual void shoot(Vector2f initPos)
    {
      // Single shot up

      // Insert new projectile into projectiles map
      Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
      proj->setScale(Vector2f(.1,.1));
      std::pair<int,Projectile*> newShot (projectiles->size(), proj);
      projectiles->insert(newShot);
   }
};

class SpreadEagle : public Weapon
{
  public:
    SpreadEagle(IntRect b, proj_map *p) : Weapon (b, p)
    {
      // Set characteristics
      potency = 1;
      speed = 20;
      cooldown = 250;
      shotTexture.loadFromFile("images/bb.png");
    }
    ~SpreadEagle()
    {

    }

    virtual void shoot(Vector2f initPos)
    {
      // Three shots, one up and two to up and side

      // Insert new projectile into projectiles map
      Projectile *proj1 = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
      proj1->setScale(Vector2f(.1,.1));
      std::pair<int,Projectile*> newShot1 (projectiles->size(), proj1);
      projectiles->insert(newShot1);

      Projectile *proj2 = new Projectile(initPos.x, initPos.y, shotTexture, speed, M_PI/4, potency);
      proj2->setScale(Vector2f(.1,.1));
      std::pair<int,Projectile*> newShot2 (projectiles->size(), proj2);
      projectiles->insert(newShot2);

      Projectile *proj3 = new Projectile(initPos.x, initPos.y, shotTexture, speed, -M_PI/4, potency);
      proj3->setScale(Vector2f(.1,.1));
      std::pair<int,Projectile*> newShot3 (projectiles->size(), proj3);
      projectiles->insert(newShot3);
   }
};

// -------------------- Enemy Weaponry -------------------- //

class PeaShooter : public Weapon
{
  public:
    PeaShooter(IntRect b, proj_map *p) : Weapon (b, p)
    {
      // Set characteristics
      potency = 1;
      speed = 20;
      cooldown = 300;
      shotTexture.loadFromFile("images/bullet.png");
    }
    ~PeaShooter()
    {

    }

    virtual void shoot(Vector2f initPos)
    {
      // Single shot down

      // Insert new projectile into projectiles map
      Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, M_PI, potency);
      proj->setScale(Vector2f(.1,.1));
      std::pair<int,Projectile*> newShot (projectiles->size(), proj);
      projectiles->insert(newShot);
   }
};
#endif
