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
  LOOP_DE_LOOP,
  TRACK_PLAYER
} MOVE_TYPE;

class Enemy : public Sprite
{
public:
  MovingHealthBar *health;
  float score;                      // How many points do we get for killing it
  float speed;                      // How fast does it move
  Weapon *weapon;                   // Enemy's current weapon
  IntRect boundaries;               // Rectangle that defines level boundaries
  Clock weaponCooldown;             // Only fire after cooldown
  MOVE_TYPE moveType;               // Which pattern enemy moves in
  Vector2f initialPos;              // Where the enemy spawns
  Texture enemyTexture;             // How it look n stuff

  proj_map *enemyProjectiles;       // Keep track of all enemy projectiles on screen
  proj_map *playerProjectiles;      // Keep track of player projectiles. If hit, lose health
  int_vec  *deadProjectiles;        // Player projectiles that have hit us. To be removed
  enemy_map *enemies;               // Keep track of all spawned enemies on screen
  bool dead = false;                // Keep track of enemy's life state

  Enemy()
  {

  }
  Enemy(proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, MOVE_TYPE m, Weapon *w, IntRect b, Vector2f spawnLoc)
  {
    // Projectile tracking
    playerProjectiles = pp;
    enemyProjectiles = ep;
    deadProjectiles = dp;
    enemies = e;

    std::cout << "Spawning enemy!" << std::endl;
    // Default enemy values, should be override in derived enemy classes
    weapon = new PeaShooter(b, ep);
    speed = 1;
    boundaries = Rect<int> (b);
    moveType = m;

    // Health
    //float ratio = .5;
    FloatRect eB = getGlobalBounds();
    Vector2f barSize(eB.width,10);
    Vector2f barLoc(eB.left,eB.top-barSize.y);
    health = new MovingHealthBar(barLoc,barSize,1);

    // Set position to spawn location
    setPosition(spawnLoc);
    setScale(2,2);
  }
  ~Enemy()
  {

  }

  void shoot (Weapon *w)
  {
    // Fire from our current position
    Vector2f pos;
    pos.x = getPosition().x + getGlobalBounds().width/2;
    pos.y = getPosition().y + getGlobalBounds().height;

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
            move (0,speed);
            //health->move(0,speed);
            break;
        default: // Default is STRAIGHT_DOWN
            move (0,speed);
            //health->move(0,speed);
            break;
    }
  }

  void checkProjectiles (RenderWindow &win)
  {
    for (std::pair<int, Projectile*> shot : *playerProjectiles)
    {
        // Workaround to appease vtable gods
        Projectile *shotObj = shot.second;
        if (shotObj && shotObj->contains(getGlobalBounds()))
        {
          // Destroy bullet
          // TODO: Maybe a special gun could pass through enemies?
          deadProjectiles->push_back(shot.first);

          loseHealth(win, 1);
        }
    }
  }

  void draw(RenderWindow &win)
  {
    // Check if we've run into any player projectiles
    checkProjectiles(win);

    // Workaround to appease vtable gods
    Sprite toDraw = *this;
    win.draw(toDraw);
    win.draw(*health);
    weapon->draw(win);
    tickMove();

    // Try to shoot every goddamn frame
    shoot(weapon);

    // Draw health
    Vector2f pos = getPosition();
    //health->setPosition(pos.x, pos.y); // TODO: Perhaps make move()
    //health->draw(win);
  }

  bool contains(FloatRect rect)
  {
    return getGlobalBounds().intersects(rect);
  }

  void loseHealth(RenderWindow &win, int amt)
  {
    // Take some damage
    health->takeDamage(win, amt);

    if (health->getCurrentHealth() <= 0)
        killSelf();
  }

  bool isDead()
  {
    return dead;
  }

  void killSelf()
  {
    dead = true;
    std::cout << "Enemy ded!" << std::endl;
  }
};

class WigWam : public Enemy
{
public:
  WigWam(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
      : Enemy(ep, pp, dp, e, STRAIGHT_DOWN, new PeaShooter(b, ep), b, spawnLoc)
  {
    enemyTexture.loadFromFile("images/bb.png");

    // Set texture
    setTexture(enemyTexture);

    // Size of the enemy
    setScale(2,2);

    // Characteristics
    speed = 1;
    health->setMaxHealth(3);
    weapon = new PeaShooter(b, ep); 

    // Set score for killing
    score = 100;
  }
  ~WigWam()
  {

  }

};
#endif
