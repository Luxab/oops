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
proj_map *ep;                       // Keep track of enemy projectiles
proj_map *pp;                       // Keep track of player projectiles
int_vec  *dp;                       // Keep track of dead projectiles
enemy_map *e;                       // Keep track of all enemies on screen

bool finishedSpawning = false;      // Set to true when wave is finished

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

    bool waveIsFinished ()
    {
      // Wave is finished when all enemies are dead and there
      // are no more to spawn
      return finishedSpawning && (e->size() <= 0);
    }
};

class WaveOne : public Wave
{

  public:
    WaveOne (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      std::vector<Enemy*> enemiesToSpawn;

      WigWam *w1 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(100,100));
      enemiesToSpawn.push_back(w1);

      WigWam *w2 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(250,300));
      enemiesToSpawn.push_back(w2);

      WigWam *w3 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(400,100));
      enemiesToSpawn.push_back(w3);

      // Create pairs for each enemy and insert into maps
      for (auto &enemy : enemiesToSpawn)
      {
        std::pair<int,Enemy*> enemyPair (e->size(), enemy);
        e->insert(enemyPair);
      }

      finishedSpawning = true;
    }

    void draw()
    {

    }
};

class WaveTwo : public Wave
{

  public:
    WaveTwo (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
    }
    ~WaveTwo ()
    {

    }

    void spawnEnemies ()
    {
      std::vector<Enemy*> enemiesToSpawn;

      WigWam *w1 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(0,100));
      enemiesToSpawn.push_back(w1);

      WigWam *w2 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(150,300));
      enemiesToSpawn.push_back(w2);

      WigWam *w3 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(200,100));
      enemiesToSpawn.push_back(w3);

      // Create pairs for each enemy and insert into maps
      for (auto &enemy : enemiesToSpawn)
      {
        std::pair<int,Enemy*> enemyPair (e->size(), enemy);
        e->insert(enemyPair);
      }

      finishedSpawning = true;
    }

    void draw()
    {

    }
};
#endif
