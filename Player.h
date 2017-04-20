//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Weapon.h"

using namespace sf;

class Player : public Sprite
{
public:
  int health; //How much damage can it take
  float speed; //How fast does it move
  Weapon *weapon; //Player's current weapon
  IntRect boundaries; // Rectangle that defines level boundaries
  Texture left, right, up, down;

  Player()
  {

  }
  Player(Texture &tin, int s, int h, FloatRect b) : Sprite(tin)
  {
    speed = s;
    health = h;
    boundaries = Rect<int> (b);
    //setPosition(rin.top, rin.left);

    left.loadFromFile("images/leftsign.png");
    right.loadFromFile("images/rightsign.png");
    up.loadFromFile("images/upsign.png");
    down.loadFromFile("images/downsign.png");

    // Default weapon
    weapon = new BBGun();
    weapon->boundingRect = boundaries;

    // Set position to middle
    setPosition(boundaries.width/2, boundaries.height/2);
  }
  ~Player()
  {

  }

  void shoot (Weapon *w)
  {
    Vector2f pos = getPosition();
    w->shoot(pos);
  }

  void tickMove()
  {
    IntRect tRec = getTextureRect();
    if (Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::Left))
    {
      movePlayer(-speed,0);
      setTexture(left);
    }
    if (Keyboard::isKeyPressed(Keyboard::D) ||
        Keyboard::isKeyPressed(Keyboard::Right))
    {
      movePlayer(speed,0);
      setTexture(right);
    }
    if (Keyboard::isKeyPressed(Keyboard::S) ||
        Keyboard::isKeyPressed(Keyboard::Down))
    {
      movePlayer(0,speed);
      setTexture(down);
    }
    if (Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::Up))
    {
      movePlayer(0,-speed);
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

    /*
    std::cout << "left: " << bounds.left << "\n"
              << "top: " << bounds.top << "\n"
              << "right: " << bounds.left + bounds.width << "\n"
              << "bottom: " << bounds.top + bounds.height << std::endl;
    */

    // Ensure bullet hasn't gone out of bounds
    if (bounds.left - boundaries.left < 5)
        move (1,0);
    else if (bounds.top  - boundaries.top  < 5)
        move (0,1);
    else if (boundaries.left + boundaries.width -
             bounds.left - bounds.width < 5)
        move (-1,0);
    else if (boundaries.top + boundaries.height -
             bounds.top - bounds.height  < 5)
        move (0,-1);
    else {
        // If not hitting a wall, move
        move (x,y);

        return true;
    }

    return false;
  }

  void draw(RenderWindow &win)
  {
    tickMove();
    weapon->draw(win);

    // Draw the weapon
    //weapon.draw();
  }

  bool contains(int mx, int my)
  {
    return getTextureRect().contains(mx,my);
  }

  void setWeapon(Weapon *w)
  {
    weapon = w;
  }
};
