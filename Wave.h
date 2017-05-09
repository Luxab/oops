/*  Created by:
 *
 *      * Hunter Brown
 *      * Duncan Klug
 *      * Andrew Morgan
 *      * HuiMin Zhang
 *
 *  Copyright (c) 2017 Really Good Software
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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
std::vector<Enemy*> waveEnemies;    // Fill this vector with all enemies you'd like to spawn during a given wave
int waveEnemiesIndex = 0;           // Iterator for wave enemy vector
double powerUpCooldown = 10000;     // How often to spawn powerups. Can be changed per wave

bool finishedSpawning = false;      // Set to true when wave is finished
bool gameStarted = false;           // Only do some stuff after game has officially started
float ratio = (float) 2 / 3;        // 2/3 ratio for spawning behind boundary line

Music *music = new Music; //music

// Random vars
std::random_device rd;
std::mt19937 rng(rd());

class Wave
{
  bool musicInit = 0;
protected:
  std::string musicFileName;
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

        if (wavePowerUps.size() != 0)
        {
            // Get random powerup
          std::uniform_int_distribution<int> uni(0,wavePowerUps.size()-1);
          auto random_index = uni(rng);

          PowerUp *randPowerup = wavePowerUps.at(random_index);

          // Set random position for each powerup
          std::uniform_int_distribution<int> pow_uni(30, boundaries.width * ratio - 50);
          auto random_width = pow_uni(rng);

          randPowerup->setPosition(random_width, 30);

          std::pair<int, PowerUp*> powerupPair(p->size(), randPowerup);
          p->insert(powerupPair);
        }
      }
    }

    void playMusic()
    {
      if (!music->getStatus())
      {
        std::cout << "Playing Wave Music" << std::endl;
        music->play();
      }
    }

    void initWaveMusic()
    {
      if (!music->openFromFile(musicFileName))
      {
        std::cout << "MUSIC LOAD FROM FILE ERROR" << std::endl;
      }
      music->setLoop(true);
    }

    void stopWaveMusic()
    {
      music->stop();
    }

    void draw(RenderWindow &win)
    {
      if (!musicInit)
      {
        musicInit = !musicInit;
        initWaveMusic();
      }
      if (gameStarted && !waveIsFinished())
      {
        playMusic();
        spawnPowerups();

        // If there are no enemies and there are still enemies to spawn,
        // spawn the next part of the wave
        if (!finishedSpawning && e->size() <= 0)
          spawnEnemies();
        else if (!finishedSpawning && e->size() <= 0 && waveEnemiesIndex >= waveEnemies.size() - 1)
          finishedSpawning = true;
      }
    }

    void setBoundaries (IntRect b)
    {
        boundaries = b;
    }

    void spawnEnemies ()
    {
      gameStarted = true;

      // Create pairs for each enemy and insert into maps
      for (waveEnemiesIndex; waveEnemiesIndex < waveEnemies.size(); ++waveEnemiesIndex)
      {
        Enemy *enemy = waveEnemies.at(waveEnemiesIndex);

        // If normal enemy, load on to screen
        if (!enemy->isBreak())
        {
          std::pair<int,Enemy*> enemyPair (e->size(), enemy);
          e->insert(enemyPair);
        } else {
          // Is break enemy, just wait for all current enemies to die
          waveEnemiesIndex++;
          break;
        }

        // Only finish wave once we reach the end of our enemies vector
        if (waveEnemiesIndex+1 >= waveEnemies.size())
          finishedSpawning = true;
      }
    }

    virtual void loadEnemiesAndPowerups () = 0;

    bool waveIsFinished ()
    {
      // Wave is finished when all enemies are dead and there
      // are no more to spawn
      powerUpCooldown = 10000; // Reset Powerup Cooldown

      return finishedSpawning && (e->size() <= 0);
    }

    void cleanup()
    {
      std::cout << "Cleaning up wave" << std::endl;
      music->stop();
      // Clear powerups
      wavePowerUps.clear();
    }
};

class WaveOne : public Wave
{
  public:
    WaveOne (IntRect b, proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
      p = pin;

      boundaries = b;

      musicFileName = "audio/Tempest2000/03 Track 3.wav";
    }
    ~WaveOne ()
    {

    }

    void loadEnemiesAndPowerups ()
    {
      // Set up enemies
      Skeltal *e1 = new Skeltal(boundaries, ep, pp, dp, e, Vector2f(100,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(e1);

      Break *b1 = new Break(boundaries, ep, pp, dp, e, Vector2f(100,100));
      waveEnemies.push_back(b1);

      WigWam *e2 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(400,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(e2);

      Break *b2 = new Break(boundaries, ep, pp, dp, e, Vector2f(200,100));
      waveEnemies.push_back(b2);

      Skeltal *e3 = new Skeltal(boundaries, ep, pp, dp, e, Vector2f(100,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(e3);

      // No powerups this round :)
    }

    void draw()
    {

    }
};

class WaveTwo : public Wave
{
  public:
    WaveTwo (IntRect b, proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e  = ein;
      p = pin;

      boundaries = b;
      musicFileName = "audio/Tempest2000/04 Track 4.wav";
    }
    ~WaveTwo ()
    {

    }

    void loadEnemiesAndPowerups ()
    {
      WigWam *w1 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(0,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(w1);

      WigWam *w2 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(150,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(w2);

      WigWam *w3 = new WigWam(boundaries, ep, pp, dp, e, Vector2f(200,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(w3);

      // Set up powerups
      wavePowerUps.push_back(new GunPowerUp(boundaries, p, pp));
      wavePowerUps.push_back(new SnackBar(boundaries, p, pp));
    }

    void draw()
    {

    }
};

// Test for the other enemy types
class WaveThree : public Wave
{
  public:
    WaveThree (IntRect b, proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e = ein;
      p = pin;

      boundaries = b;
      musicFileName = "audio/Tempest2000/05 Track 5.wav";
    }
    ~WaveThree()
    {
      // Do Nothing
    }

    void loadEnemiesAndPowerups ()
    {
      // Set up enemies
      SnipeHunt *s1 = new SnipeHunt(boundaries, ep, pp, dp, e, Vector2f(0,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(s1);

      BigGuns *b1 = new BigGuns(boundaries, ep, pp, dp, e, Vector2f(250,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(b1);

      RunGun *r1 = new RunGun(boundaries, ep, pp, dp, e, Vector2f(400,ENEMY_SPAWN_LINE));
      waveEnemies.push_back(r1);

      // Set up powerups
      wavePowerUps.push_back(new GunPowerUp(boundaries, p, pp));
      wavePowerUps.push_back(new SnackBar(boundaries, p, pp));
    }

    void spawnEnemies()
    {

    }
};

class WaveFour : public Wave
{
  public:
    WaveFour (IntRect b, proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e = ein;
      p = pin;

      boundaries = b;
      musicFileName = "audio/Tempest2000/01 Thermal Resolution.wav";

      powerUpCooldown = 500;
    }
    ~WaveFour()
    {
      // Do Nothing
    }

    void loadEnemiesAndPowerups ()
    {
      std::uniform_real_distribution<float> randx(0, 400);

      for(int i = 0; i < 5; i++) {
        waveEnemies.push_back(new Skeltal(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
        wavePowerUps.push_back(new Doritos(boundaries, p, pp));
      }
    }

    void spawnEnemies()
    {

    }
};

class WaveProcedural : public Wave
{
  float difficulty;
  public:
    WaveProcedural (IntRect b, proj_map *ppin, proj_map *epin, int_vec *dpin, enemy_map *ein, pow_map *pin, float difficulty)
    {
      pp = ppin;
      ep = epin;
      dp = dpin;
      e = ein;
      p = pin;
      this->difficulty = difficulty;

      boundaries = b;
      
      std::uniform_int_distribution<int> shuffle_music(1, 12);
      switch(shuffle_music(rng)) {
        case 2: musicFileName = "audio/Tempest2000/02 Track 2.wav"; break;
        case 3: musicFileName = "audio/Tempest2000/03 Track 3.wav"; break;
        case 4: musicFileName = "audio/Tempest2000/04 Track 4.wav"; break;
        case 5: musicFileName = "audio/Tempest2000/05 Track 5.wav"; break;
        case 6: musicFileName = "audio/Tempest2000/06 Track 6.wav"; break;
        case 7: musicFileName = "audio/Tempest2000/07 Track 7.wav"; break;
        case 8: musicFileName = "audio/Tempest2000/08 Track 8.wav"; break;
        case 9: musicFileName = "audio/Tempest2000/09 Track 9.wav"; break;
        case 10: musicFileName = "audio/Tempest2000/10 Track 10.wav"; break;
        case 11: musicFileName = "audio/Tempest2000/11 Track 11.wav"; break;
        case 12: musicFileName = "audio/Tempest2000/12 Track 12.wav"; break;
        default: musicFileName = "audio/Tempest2000/01 Thermal Resolution.wav"; break;
      }
    }
    ~WaveProcedural()
    {
      // Do Nothing
    }

    void loadEnemiesAndPowerups()
    {
      std::uniform_real_distribution<float> randx(0, 400);
      std::uniform_int_distribution<int> rand_enemy(0, 5);
      std::uniform_int_distribution<int> rand_powerup(0, 2);
      int d = 0;
      int lastbreak = 0;

      switch(rand_powerup(rng)) {
        case 0:
          wavePowerUps.push_back(new Doritos(boundaries, p, pp));
	  break;
        case 1:
          wavePowerUps.push_back(new GunPowerUp(boundaries, p, pp));
          break;
        case 2:
          wavePowerUps.push_back(new SnackBar(boundaries, p, pp));
      }

      while(d <= difficulty) {
        switch(rand_enemy(rng)) {
          case 0:
          {
            waveEnemies.push_back(new CircleShot(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            break;
          }
          case 1:
          {
            waveEnemies.push_back(new SnipeHunt(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            d++;
            break;
          }
          case 2:
          {
            waveEnemies.push_back(new BigGuns(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            d++;
            break;
          }
          case 3:
          {
            waveEnemies.push_back(new RunGun(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            d++;
            break;
          }
          case 4:
          {
            waveEnemies.push_back(new Skeltal(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            d++;
            break;
          }
          case 5:
          {
            waveEnemies.push_back(new WigWam(boundaries, ep, pp, dp, e, Vector2f(randx(rng), ENEMY_SPAWN_LINE)));
            d++;
            break;
          }
        }
        if(waveEnemies.size() - lastbreak > 5) {
          waveEnemies.push_back(new Break(boundaries, ep, pp, dp, e, Vector2f(0,0)));
          lastbreak = waveEnemies.size();
        }
      }
    }

    void spawnEnemies()
    {

    }
};
#endif
