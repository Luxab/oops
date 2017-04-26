//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_map>

typedef std::unordered_map<int, Enemy*> enemy_map;
typedef std::unordered_map<int, Projectile*> proj_map;

Clock spawnerClock;
IntRect boundaries;
enemy_map *enemies;
proj_map *projectiles;
int enemyCount; // Increments per enemy added

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
    WaveOne (proj_map *p, enemy_map *e)
    {
      projectiles = p;
      enemies = e;
    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      // Insert new projectile into projectiles map
      WigWam *w1 = new WigWam(boundaries, Vector2f(200,200), projectiles);
      std::pair<int,Enemy*> newEnemy (enemyCount++, (Enemy*)w1);
      enemies->insert(newEnemy);
    }

    void draw()
    {

    }
};
#endif
