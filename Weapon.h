//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
  IntRect boundaries;           // If projectiles leave this rect, delete them
  proj_map *projectiles;        // Keeps track of all projectiles on screen

  Sound shotSound;              // Sound that plays when weapon is shot
  SoundBuffer shotSoundBuffer;  // Buffer for shot sound

  Weapon(IntRect b, proj_map *p)
  {
    boundaries = b;
    projectiles = p;

    shotSound.setBuffer(shotSoundBuffer);
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
      if (!boundaries.intersects(Rect<int>(shot.second->getGlobalBounds())))
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
      shotSoundBuffer.loadFromFile("audio/shot_sound.wav");
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

      shotSound.play();
   }
};

// Very large bullet
class LargeBullet : public Weapon
{
  public:
    LargeBullet(IntRect b, proj_map *p) : Weapon (b, p)
    {
        potency = 3;
        speed = 20;
        cooldown = 750;
        shotTexture.loadFromFile("images/bullet.png");
    }
    ~LargeBullet()
    {
        // Do nothing
    }

    virtual void shoot(Vector2f initPos)
    {
      Projectile *proj1 = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
      proj1->setScale(Vector2f(.3,.3));
      std::pair<int,Projectile*> newShot1 (projectiles->size(), proj1);
      projectiles->insert(newShot1);

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

// Sniper has higher damage and speed, but much longer cooldown than the PeaShooter
class Sniper : public Weapon
{
  public:
    Sniper(IntRect b, proj_map *p) : Weapon (b, p)
    {
        potency = 10;
        speed = 40;
        cooldown = 1000;
        shotTexture.loadFromFile("images/bullet.png");
    }
    ~Sniper()
    {
        // Do nothing
    }

    virtual void shoot(Vector2f initPos)
    {
        // One shot down, as in PeaShooter
        Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
        proj->setScale(Vector2f(.2, .2));
        std::pair<int,Projectile*> newShot (projectiles->size(), proj);
        projectiles->insert(newShot);
    }
};

// Wide Bullet Gun
class WideGun : public Weapon
{
  public:
    WideGun(IntRect b, proj_map *p) : Weapon (b, p)
    {
        potency = 1;
        speed = 2;
        cooldown = 3000;
        shotTexture.loadFromFile("images/bullet.png");
    }
    ~WideGun()
    {
        // Do nothing
    }

    virtual void shoot(Vector2f initPos)
    {

        // Three projectiles, straight down
        Projectile *proj1 = new Projectile(initPos.x, initPos.y, shotTexture, speed, 0, potency);
        proj1->setScale(Vector2f(.6, .6));
        std::pair<int,Projectile*> newShot1 (projectiles->size(), proj1);
        projectiles->insert(newShot1);
    }

};

// Shotgun for enemies
class Shotter : public Weapon
{
  public:
    Shotter(IntRect b, proj_map *p) : Weapon (b, p)
    {
        potency = 1;
        speed = 2;
        cooldown = 3000;
        shotTexture.loadFromFile("images/bullet.png");
    }
    ~Shotter()
    {
        // Do nothing
    }

    virtual void shoot(Vector2f initPos)
    {
        // Five Projectiles at angles
        int bulletAmt = 6;
        double scale = 0.1;
        for (int i = 0; i < bulletAmt; i++)
        {
            Projectile *proj = new Projectile(initPos.x, initPos.y, shotTexture, speed, -(M_PI/2) + (2*i/M_PI), potency);
            proj->setScale(Vector2f(scale, scale));
            std::pair<int,Projectile*> newShot (projectiles->size(), proj);
            projectiles->insert(newShot);
        }
    }
};

#endif
