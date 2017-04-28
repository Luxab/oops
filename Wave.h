//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_map>
#include <vector>

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
      std::vector<Enemy*> enemiesToSpawn;

      WigWam *w1 = new WigWam(boundaries, enemyProjectiles, playerProjectiles, enemies, Vector2f(100,100));
      enemiesToSpawn.push_back(w1);

      WigWam *w2 = new WigWam(boundaries, enemyProjectiles, playerProjectiles, enemies, Vector2f(250,300));
      enemiesToSpawn.push_back(w2);

      WigWam *w3 = new WigWam(boundaries, enemyProjectiles, playerProjectiles, enemies, Vector2f(400,100));
      enemiesToSpawn.push_back(w3);

      // Create pairs for each enemy and insert into maps
      for (auto &enemy : enemiesToSpawn)
      {
        std::pair<int,Enemy*> enemyPair (enemies->size(), enemy);
        enemies->insert(enemyPair);
      }
    }

    void draw()
    {

    }
};
#endif
