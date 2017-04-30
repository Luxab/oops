//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "HealthBar.h"
#include <vector>

using namespace sf;

class Player : public Sprite
{
public:
  HealthBar *health;
  //int health;                         // How much damage can it take
  float speed;                          // How fast does it move
  Weapon *weapon;                       // Player's current weapon
  IntRect boundaries;                   // Rectangle that defines level boundaries
  float rightSide;
  float bottomSide;
  Texture left, right, up, down;
  Clock weaponCooldown;
  proj_map *enemyProjectiles;           // Keep track of all enemy projectiles on screen
  proj_map *playerProjectiles;
  enemy_map *enemies;
  bool dead = false;

  Player()
  {

  }
  Player(Texture &tin, proj_map *pp, proj_map *ep, enemy_map *e, int s, FloatRect b) : Sprite(tin)
  {
    // Projectile tracking
    playerProjectiles = pp;
    enemyProjectiles = ep;
    enemies = e;

    // Player characteristics
    speed = s;
    boundaries = Rect<int> (b);
    rightSide = boundaries.left+boundaries.width;
    bottomSide = boundaries.top+boundaries.height;

    float ratio = .5;
    Vector2f barLoc(b.left+b.width+2,2);
    Vector2f barSize(b.width*ratio,b.height*.05);
    health = new HealthBar(barLoc,barSize,5);

    left.loadFromFile("images/Skateboard_Left.png");
    right.loadFromFile("images/Skateboard_Right.png");
    up.loadFromFile("images/Skateboard_Forward.png");
    down.loadFromFile("images/Skateboard_Forward.png");
    //setTexture(up);

    // Default weapon
    weapon = new SpreadEagle(boundaries, playerProjectiles);

    // Set position to middle
    setPosition(boundaries.width/2, boundaries.height/2);
    setScale(2,2);
  }
  ~Player()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = getPosition();
    
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

    bool lMov =  Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::Left);
    bool rMov = Keyboard::isKeyPressed(Keyboard::D) ||
        Keyboard::isKeyPressed(Keyboard::Right);
    bool dMov = Keyboard::isKeyPressed(Keyboard::S) ||
        Keyboard::isKeyPressed(Keyboard::Down);
    bool uMov = Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::Up);

    float tSpeed = speed;
    if(lMov+rMov+dMov+uMov>1)
      tSpeed*=.5;

    if (lMov)
    {
      movePlayer(-tSpeed,0);
      setTexture(left);
    }
    if (rMov)
    {
      movePlayer(tSpeed,0);
      setTexture(right);
    }
    if (dMov)
    {
      movePlayer(0,tSpeed);
      setTexture(down);
    }
    if (uMov)
    {
      movePlayer(0,-tSpeed);
      setTexture(up);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
      shoot(weapon);
    }
    //Move in {direction} at {speed}
    //setPosition(x,y);

    // Apply changes
    setTextureRect(tRec);
  }

  // Move player and check boundaries
  // Returns true if move was successful, false if otherwise
  bool movePlayer (float x, float y)
  {
    FloatRect bounds = getGlobalBounds();
    //bounds -> player bounds
    //boundaries -> level bounds

    // float rightSide = boundaries.left+boundaries.width;
    // float bottomSide = boundaries.top+boundaries.height;


    // std::cout << "left: " << bounds.left << "\n"
    //           << "top: " << bounds.top << "\n"
    //           << "right: " << bounds.left + bounds.width << "\n"
    //           << "bottom: " << bounds.top + bounds.height << std::endl;


    // Ensure player hasn't gone out of bounds
    if (bounds.left+x < boundaries.left)
    {
      move(boundaries.left,0);
    }
    else if (bounds.top+y<boundaries.top)
    {
      move(0,boundaries.top);
    }
    else if (bounds.left+bounds.width+x > rightSide)
    {
      move(rightSide-bounds.left-bounds.width,0);
    }
    else if (bounds.top+bounds.height+y > bottomSide)
    {
      move(0,bottomSide-bounds.top-bounds.height);
    }
    else
    {
      // If not hitting a wall, move
      move (x,y);

      return true;
    }

    return false;
  }

  void checkProjectiles (RenderWindow &win)
  {
    for (std::pair<int, Projectile*> shot : *enemyProjectiles)
    {
      // Workaround to appease vtable gods
      Projectile *shotObj = shot.second;
      if (shotObj->contains(getGlobalBounds()))
      {
        loseHealth(win,1);
      }
    }
  }

  void draw(RenderWindow &win)
  {
    // Check if we've run into any enemy projectiles
    checkProjectiles(win);

    tickMove();
    weapon->draw(win);
    health->draw(win);

    // Draw the weapon
    //weapon.draw();
  }

  bool contains(FloatRect rect)
  {
    return getGlobalBounds().intersects(rect);
  }

  void loseHealth(RenderWindow &win, int amt)
  {
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
    std::cout << "You ded!" << std::endl;
  }

  void setWeapon(Weapon *w)
  {
    weapon = w;
  }
};

#endif
