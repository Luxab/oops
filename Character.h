//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef CHARACTER_H
#define CHARACTER_H

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

// Needs to basically be what enemy was,
// While enemy needs to become a wrapper
// Like the Weapon class is.
class Character : public Sprite
{
public:
  MovingHealthBar *health;
  float speed;                  // How fast does it move
  Weapon *weapon;               // Enemy's current weapon
  IntRect boundaries;           // Rectangle that defines level boundaries
  Clock weaponCooldown;         // Only fire after cooldown
  MOVE_TYPE moveType;           // Which pattern enemy moves in
  Vector2f initialPos;          // Where the enemy spawns
  Texture enemyTexture;         // How it look n stuff

  Character()
  {

  }
  Character(proj_map *ep, proj_map *pp, enemy_map *e, int h, int s, MOVE_TYPE m, Weapon *w, IntRect b, Vector2f spawnLoc) : Sprite(tin)
  {
    // Projectile tracking
    playerProjectiles = pp;
    enemyProjectiles = ep;
    enemies = e;

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
    initialPos = spawnLoc;
    std::cout << "IPos: " << initialPos.x << "," << initialPos.y << std::endl;
    setPosition(initialPos);
    setScale(2,2);
  }
  ~Enemy()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = initialPos;

    // Only fire if timer is past cooldown
    if (weaponCooldown.getElapsedTime().asMilliseconds() > w->cooldown)
    {
      w->shoot(pos);
      weaponCooldown.restart();
    }
  }

  void tickMove()
  {
    IntRect tRec = getTextureRect();

    // Just move down
    switch (moveType) {
        case STRAIGHT_DOWN:
            move (0,-speed);
            break;
    }
  }

  void draw(RenderWindow &win)
  {
    // Workaround to appease vtable gods
    Sprite toDraw = *this;
    win.draw(toDraw);

    std::cout << "Here?" << std::endl;
    weapon->draw(win);
    std::cout << "Hereeeee?" << std::endl;
    tickMove();

    // Try to shoot every goddamn frame
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

class WigWam : public Enemy
{
public:
  WigWam(IntRect b, proj_map *ep, proj_map *pp, enemy_map *e, Vector2f spawnLoc) 
      : Enemy(ep, pp, e, 1, 5, STRAIGHT_DOWN, new BBGun(b, ep), b, spawnLoc);
  {
    enemyTexture.loadFromFile("images/bb.png");
  }
  ~WigWam()
  {

  }

};
#endif
