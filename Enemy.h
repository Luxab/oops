//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "HealthBar.h"
#include <vector>
#include <cmath> // For LOOP_DE_LOOP

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
  Clock movementClock;              // Used for LOOP_DE_LOOP MOVE_TYPE
  MOVE_TYPE moveType;               // Which pattern enemy moves in
  Vector2f initialPos;              // Where the enemy spawns
  Texture enemyTexture;             // How it look n stuff

  proj_map *enemyProjectiles;       // Keep track of all enemy projectiles on screen
  proj_map *playerProjectiles;      // Keep track of player projectiles. If hit, lose health
  int_vec  *deadProjectiles;        // Player projectiles that have hit us. To be removed
  enemy_map *enemies;               // Keep track of all spawned enemies on screen
  bool dead = false;                // Keep track of enemy's life state
  bool deathProcess = false;        // Starts the death animation if true
  float ratio = (float) 2 / 3;      // Ratio for boundary lines

  bool mvDirect = false;            // For ZIG_ZAG: false if moving left, true if moving right
  int cirRad = 50;                  // Radius of circle for LOOP_DE_LOOP

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
      // Do Nothing
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

    // Cannot initialize new variables inside the switch statement, so doing so here
    float cenX, cenY, xPosCir, yPosCir;
    float x1,y1,x2,y2,movebyX,movebyY,theta1,theta2;

    // Just move down
    switch (moveType) {
        case STRAIGHT_DOWN:
            move (0,speed);
            //health->move(0,speed);
            break;
        case ZIG_ZAG:
            // Check if we are hitting the boundary
            if (getPosition().x > boundaries.left + boundaries.width * ratio
                    - getGlobalBounds().width)
            {
                // Checks right boundary - note that this is not the black bar currently
                mvDirect = !mvDirect;
            }
            else if (getPosition().x < boundaries.left)
            {
                // Check left boundary
                mvDirect = !mvDirect;
            }

            // Choose our movement direction
            if (mvDirect == false)
            {
                move(-speed, 0); // Move to the left
            }
            else
            {
                move(speed, 0); // Move to the right
            }
            break;
        case LOOP_DE_LOOP:
            // We want to move along the tangent of a circle on which we are starting at the rightmost point
            // We need our current location relative to the center of the circle we are on

            // Grab the center of the circle the enemy is orbiting around
            cenX = initialPos.x - cirRad;
            cenY = initialPos.y;

            // Make is a circle with with (0,0) at center
            xPosCir = getPosition().x - cenX;
            yPosCir = getPosition().y - cenY;

            // This is the offset for the current position and the next position
            theta1 = 2.0f * M_PI * movementClock.getElapsedTime().asMilliseconds() / 360.0f;
            theta2 = 2.0f * M_PI * (movementClock.getElapsedTime().asMilliseconds() + 10) / 360.0f;

            // x1, y1 is current location relative to circle center, while x2 and y2 are location we want to move to
            x1 = cirRad * cosf(theta1) + initialPos.x;
            y1 = cirRad * sinf(theta1) + initialPos.y;
            x2 = cirRad * cosf(theta2) + initialPos.x;
            y2 = cirRad * sinf(theta2) + initialPos.y;

            // Movement offset for move
            movebyX = x2 - x1;
            movebyY = y2 - y1;

            // Move along the tangent to the circle
            move(movebyX, movebyY);

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

  virtual void draw(RenderWindow &win)
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
        deathAnimation();
  }

  bool isDead()
  {
    return dead;
  }

  virtual void deathAnimation()
  {
    dead = true;
  }
};

// Basic enemy
class WigWam : public Enemy
{
  public:
    // Constructor
    WigWam(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
        : Enemy(ep, pp, dp, e, LOOP_DE_LOOP, new PeaShooter(b, ep), b, spawnLoc)
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
      score = 25;
    }
    // Destructor
    ~WigWam()
    {
        // Do Nothing
    }
};

class Skeltal : public Enemy
{
  bool walk = 1;
  bool deathProcess = 0;
  Texture walk1;
  Texture walk2;
  Texture deadTexture;
public:
  Clock aClock;
  bool death;
  Skeltal(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
      : Enemy(ep, pp, dp, e, STRAIGHT_DOWN, new PeaShooter(b, ep), b, spawnLoc)
  {
    //setPosition(spawnLoc);
    walk1.loadFromFile("images/skel_walk1.png");
    walk2.loadFromFile("images/skel_walk2.png");
    deadTexture.loadFromFile("images/skel_death.png");
    setTexture(walk1);
    score = 50;
  }

  void walkAnimation()
  {
    if (aClock.getElapsedTime().asSeconds()>.5)
    {
      if (walk)
      {
        setTexture(walk1);
      }
      else
      {
        setTexture(walk2);
      }
      walk = !walk;
      aClock.restart();
    }
  }

  virtual void deathAnimation()
  {
    if (aClock.getElapsedTime().asSeconds()<.5)
    {
      setTexture(deadTexture);
      deathProcess = true;
    }
    else
    {
      dead = 1;
    }
  }

  virtual void draw(RenderWindow &win)
  {
    // Check if we've run into any player projectiles
    checkProjectiles(win);

    if (!deathProcess)
    {
      walkAnimation();
    }
    else
    {
      deathAnimation();
    }

    // Workaround to appease vtable gods
    Sprite toDraw = *this;
    win.draw(toDraw);
    tickMove();
  }
};

// Sniper
// Moves in a zig-zag pattern
class SnipeHunt : public Enemy
{
  public:
    // Constructor
    SnipeHunt(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
        : Enemy(ep, pp, dp, e, ZIG_ZAG, new Sniper(b, ep), b, spawnLoc)
      {
          enemyTexture.loadFromFile("images/bb.png");
          setTexture(enemyTexture);
          setScale(2, 2);

          speed = 1;
          health->setMaxHealth(3);
          weapon = new Sniper(b, ep);

         score = 100;
     }
     // Destructor
     ~SnipeHunt()
     {
         // Do Nothing
     }
};

// Big Bullet
class BigGuns : public Enemy
{
  public:
    // Constructor
    BigGuns(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
        : Enemy(ep, pp, dp, e, STRAIGHT_DOWN, new WideGun(b, ep), b, spawnLoc)
    {
        enemyTexture.loadFromFile("images/bb.png");
        setTexture(enemyTexture);
        setScale(2, 2);

        speed = 1;
        health->setMaxHealth(3);
        weapon = new WideGun(b, ep);

        score = 100;
    }
    // Destructor
    ~BigGuns()
    {
        // Do Nothing
    }
};

// Shotgun
class RunGun : public Enemy
{
  public:
    // Constructor
    RunGun(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
        : Enemy(ep, pp, dp, e, STRAIGHT_DOWN, new Shotter(b, ep), b, spawnLoc)
    {
        enemyTexture.loadFromFile("images/bb.png");
        setTexture(enemyTexture);
        setScale(2, 2);

        speed = 1;
        health->setMaxHealth(3);
        weapon = new Shotter(b, ep);

        score = 100;
    }
    // Destructor
    ~RunGun()
    {
        // Do Nothing
    }
};

// Moves in Circle while using shotter shotgun
class CircleShot : public Enemy
{
  public:
    // Constructor
    CircleShot(IntRect b, proj_map *ep, proj_map *pp, int_vec *dp, enemy_map *e, Vector2f spawnLoc)
        : Enemy(ep, pp, dp, e, LOOP_DE_LOOP, new Shotter(b, ep), b, spawnLoc)
    {
        enemyTexture.loadFromFile("images/bb.png");
        setTexture(enemyTexture);
        setScale(2, 2);

        speed = 1;
        health->setMaxHealth(2); // Slightly lower health than normal
        weapon = new Shotter(b, ep);

        score = 100;
    }
    // Destructor
    ~CircleShot()
    {
        // Do Nothing
    }
};
#endif
