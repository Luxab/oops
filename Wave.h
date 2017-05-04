//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "PowerUp.h"
#include <unordered_map>
#include <vector>
#include <random>

Clock enemySpawnClock;              // Spawn enemies on a timer
Clock powerupSpawnClock;            // Spawn powerups on a timer
IntRect boundaries;                 // Boundaries of play area
proj_map *ep;                       // Keep track of enemy projectiles
proj_map *pp;                       // Keep track of player projectiles
int_vec  *dp;                       // Keep track of dead projectiles
enemy_map *e;                       // Keep track of all enemies on screen
pow_map *p;                         // Keep track of all powerups

// Random vars
std::random_device rd;
std::mt19937 rng(rd());

std::vector<PowerUp*> wavePowerUps; // Fill this vector with all powerups you'd like to spawn during a given wave
double powerUpCooldown = 10000;     // How often to spawn powerups. Can be changed

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

    void spawnPowerups()
    {
      if (powerupSpawnClock.getElapsedTime().asMilliseconds() > powerUpCooldown)
      {
        std::cout << "Spawning powerups" << std::endl;
        powerupSpawnClock.restart();

        // Get random powerup
        std::uniform_int_distribution<int> uni(0,wavePowerUps.size());
        auto random_index = uni(rng);
        
        if (wavePowerUps.size() != 0)
        {
            PowerUp *randPowerup = wavePowerUps.at(random_index);

            std::pair<int, PowerUp*> powerupPair(p->size(), randPowerup);
            p->insert(powerupPair);
        }
      }
    }

    void draw(RenderWindow &win)
    {
      powerupSpawnClock.restart();
      spawnPowerups();
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
    WaveOne (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
      p = pin;
    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      std::vector<Enemy*> enemiesToSpawn;

      Skeltal *w1 = new Skeltal(boundaries, ep, pp, dp, e, Vector2f(100,100));
      enemiesToSpawn.push_back(w1);

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
    WaveTwo (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
      p = pin;
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

// Test for the other enemy types
class WaveThree : public Wave
{
  public:
    WaveThree (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e = ein;
      p = pin;
    }
    ~WaveThree()
    {
      // Do Nothing
    }

    void spawnEnemies()
    {
      std::vector<Enemy*> enemiesToSpawn;

      SnipeHunt *s1 = new SnipeHunt(boundaries, ep, pp, dp, e, Vector2f(0,100));
      enemiesToSpawn.push_back(s1);

      BigGuns *b1 = new BigGuns(boundaries, ep, pp, dp, e, Vector2f(250,300));
      enemiesToSpawn.push_back(b1);

      RunGun *r1 = new RunGun(boundaries, ep, pp, dp, e, Vector2f(400,100));
      enemiesToSpawn.push_back(r1);

      for(auto &enemy : enemiesToSpawn)
      {
          std::pair<int,Enemy*> enemyPair (e->size(), enemy);
          e->insert(enemyPair);
      }

       finishedSpawning = true;
    }
};

class WaveProcedural : public Wave
{
  float difficulty;
  public:
    WaveProcedural (proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin, float difficulty)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e = ein;
      p = pin;
      this->difficulty = difficulty;
    }
    ~WaveProcedural()
    {
      // Do Nothing
    }

    void spawnEnemies()
    {
      std::vector<Enemy*> enemiesToSpawn;
      std::uniform_real_distribution<float> randx(0, 400);
      std::uniform_real_distribution<float> randy(100, 500);

      int d = 0;
      while(d <= difficulty) {
        SnipeHunt *sh = new SnipeHunt(boundaries, ep, pp, dp, e, Vector2f(randx(rng), randy(rng)));
        if(sh->score <= difficulty-d) {
          enemiesToSpawn.push_back(sh);
	  d += sh->score;
        }
	else {
	  delete sh;
	  break;
	}
        BigGuns *bg = new BigGuns(boundaries, ep, pp, dp, e, Vector2f(randx(rng), randy(rng)));
        if(bg->score <= difficulty-d) {
          enemiesToSpawn.push_back(bg);
	  d += bg->score;
        }
	else {
          delete bg;
	  break;
	}

        RunGun *rg = new RunGun(boundaries, ep, pp, dp, e, Vector2f(randx(rng), randy(rng)));
        if(rg->score <= difficulty-d) {
          enemiesToSpawn.push_back(rg);
	  d += rg->score;
        }
	else {
          delete rg;
	  break;
	}

        Skeltal *s = new Skeltal(boundaries, ep, pp, dp, e, Vector2f(randx(rng), randy(rng)));
        if(s->score <= difficulty-d) {
          enemiesToSpawn.push_back(s);
	  d += s->score;
        }
        else {
          delete s;
	  break;
	}

        WigWam *ww = new WigWam(boundaries, ep, pp, dp, e, Vector2f(randx(rng), randy(rng)));
        if(ww->score <= difficulty-d) {
          enemiesToSpawn.push_back(ww);
	  d += ww->score;
        }
        else {
          delete ww;
	  break;
	}
      }

      for(auto &enemy : enemiesToSpawn)
      {
          std::pair<int,Enemy*> enemyPair (e->size(), enemy);
          e->insert(enemyPair);
      }

       finishedSpawning = true;
    }
};
#endif
