//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_map>

Clock spawnerClock;
IntRect boundaries;
std::unordered_map<int, Enemy*> enemies;
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
      std::vector<int> toBeDeleted;
      for (auto &enemy : enemies)
      {
        win.draw(*enemy.second);
        enemy.second->tickMove();

        /*
        // If enemy is colliding with a player bullet, kill them
        if (!boundingRect.intersects(Rect<int>(enemy.second->getGlobalBounds())))
        {
          // If collided, remove from hashmap
          toBeDeleted.push_back(shot.first);

          // Increase player score
          ...
        }
        */
      }

      // Delete all projectiles that went off-screen
      for (auto &enemyKey : toBeDeleted)
      {
        enemies.erase(enemyKey);
      }
  }

    void setBoundaries (IntRect b)
    {
        boundaries = b;
    }

    virtual void spawnEnemies ()
    {

    }
};

class WaveOne : public Wave
{
  
  public:
    WaveOne ()
    {

    }
    ~WaveOne ()
    {

    }

    void spawnEnemies ()
    {
      // Insert new projectile into projectiles map
      WigWam *w1 = new WigWam(boundaries, Vector2f(20,20));
      std::pair<int,Enemy*> newEnemy (enemyCount++, (Enemy*)w1);
      enemies.insert(newEnemy);
    }

    void draw()
    {

    }
};
#endif
