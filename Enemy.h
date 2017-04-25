//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "HealthBar.h"
#include <vector>

using namespace sf;

typedef enum MOVE_TYPE
{
    STRAIGHT_DOWN,
    ZIG_ZAG,
    LOOP_DE_LOOP
} MOVE_TYPE;

Texture EnemyTexture; // How it look n stuff

// This class is spawned by the others below
class Enemy : public Sprite
{
public:
  EnemyHealthBar *health;
  float speed; //How fast does it move
  Weapon *weapon; //Enemy's current weapon
  IntRect boundaries; // Rectangle that defines level boundaries
  Clock weaponCooldown;
  MOVE_TYPE moveType;

  Enemy()
  {

  }
  Enemy(Texture &tin, int h, int s, MOVE_TYPE m, Weapon *w, IntRect b, Vector2f spawnLoc) : Sprite(tin)
  {
    std::cout << "Spawning enemy!" << std::endl;
    weapon = w;
    speed = s;
    boundaries = Rect<int> (b);

    // Health
    float ratio = .5;
    Vector2f barLoc(b.left+b.width+2,2);
    Vector2f barSize(b.width*ratio,b.height*.05);
    health = new EnemyHealthBar(barLoc,barSize,h);

    // Set texture
    setTexture(tin);

    // Set position to spawn location
    std::cout << "SPos: " << spawnLoc.x << "," << spawnLoc.y << std::endl;
    setPosition(spawnLoc);
    setScale(2,2);
  }
  ~Enemy()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = getPosition();

    // Only fire if timer is past cooldown
    if (weaponCooldown.getElapsedTime().asMilliseconds() > w->cooldown)
    {
        w->shoot(pos);
        weaponCooldown.restart();
    }
  }

  void tickMove()
  {
    return;
    IntRect tRec = getTextureRect();

    // Just move down!
    switch (moveType) {
        case STRAIGHT_DOWN:
            move (0,-speed);
            break;
    }
  }

  void draw(RenderWindow &win)
  {
    win.draw(*this);
    tickMove();
    weapon->draw(win);

    // Try to shoot every goddamn second
    shoot(weapon);

    // Draw health
    Vector2f pos = getPosition();
    //health->setPosition(pos.x, pos.y); // TODO: Perhaps make move()
    //health->draw(win);
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

class WigWam: public Enemy
{
public:
  WigWam(IntRect b, Vector2f spawnLoc)
  {
    Texture eTexture;
    eTexture.loadFromFile("images/bb.png");

    Enemy(eTexture, 1, 5, STRAIGHT_DOWN, new BBGun(), b, spawnLoc);
  }
  ~WigWam()
  {

  }

};
#endif
