//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_map>

Clock spawnerClock;                 // Spawn enemies on a timer
IntRect boundaries;                 // Boundaries of play area
proj_map *enemyProjectiles;         // Keep track of enemy projectiles
proj_map *playerProjectiles;        // Keep track of player projectiles
enemy_map *enemies;                 // Keep track of all enemies on screen

class Wave
{
  public:
    Wave ()
    {

    }
    ~Wave ()
    {

    }

    void draw(RenderWindow &win)
    {

    }

    void setBoundaries (IntRect b)
    {
        boundaries = b;
    }

    virtual void spawnEnemies () = 0;
};

class WaveOne : public Wave
{

  public:
    WaveOne (proj_map *pp, proj_map *ep, enemy_map *e)
    {
      playerProjectiles = pp;
      enemyProjectiles = ep;
      enemies = e;
    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      // TODO: Enemy needs to be a parent to all these types yoo
      // Insert new projectile into projectiles map
      WigWam *w1 = new WigWam(boundaries, enemyProjectiles, playerProjectiles, enemies, Vector2f(200,200));
      std::pair<int,Enemy*> newEnemy (enemies->size(), w1);
      enemies->insert(newEnemy);
    }

    void draw()
    {

    }
};
#endif
