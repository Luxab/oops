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

std::vector<PowerUp*> wavePowerUps; // Fill this vector with all powerups you'd like to spawn during a given wave
double powerUpCooldown = 5000;      // How often to spawn powerups. Can be changed

bool finishedSpawning = false;      // Set to true when wave is finished
float ratio = (float) 2 / 3;        // 2/3 ratio for spawning behind boundary line

bool gameStarted = false;           // Only do some stuff after game has officially started

// Random vars
std::random_device rd;
std::mt19937 rng(rd());

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
        powerupSpawnClock.restart();

        // Get random powerup

        std::uniform_int_distribution<int> uni(0,wavePowerUps.size()-1);
        auto random_index = uni(rng);

        if (wavePowerUps.size() != 0)
        {
            PowerUp *randPowerup = wavePowerUps.at(random_index);

            // Set random position for each powerup
            std::uniform_int_distribution<int> pow_uni(30, boundaries.width * ratio - 50);
            auto random_width = pow_uni(rng);
            std::cout << "Spawning powerups at " << random_width << "," << "30" << std::endl;

            randPowerup->setPosition(random_width, 30);

            std::pair<int, PowerUp*> powerupPair(p->size(), randPowerup);
            p->insert(powerupPair);
        }
      }

    }

    void draw(RenderWindow &win)
    {
      if (gameStarted)
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

      wavePowerUps.push_back(new SnackBar(boundaries, p, ppin));
      wavePowerUps.push_back(new GunPowerUp(boundaries, p, ppin));
    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      gameStarted = true;

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

#endif
