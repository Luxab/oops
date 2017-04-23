//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Weapon.h"

using namespace sf;

class HealthBar : public RectangleShape
{
  int maxHealth;
  int health;
  int x,y,bSize;
  Color bColor;
  int bWidth, bHeight; //width and height of the background.

public:

  HealthBar(Vector2f location = Vector2f(0,0), Vector2f size = Vector2f(100,30), int mh=5) : RectangleShape(size)
  {
    maxHealth = mh;
    health = maxHealth;
    setPosition(location.x,location.y);
    setFillColor(Color(234,0,0));
  }
  ~HealthBar()
  {

  }

  void takeDamage(RenderWindow &window)
  {
    health--;
    setSize(Vector2f(getSize().x*(health/maxHealth),getSize().y));
  }
};

class Player : public Sprite
{
public:
  HealthBar *health;
  //int health; //How much damage can it take
  float speed; //How fast does it move
  Weapon *weapon; //Player's current weapon
  IntRect boundaries; // Rectangle that defines level boundaries
  float rightSide;
  float bottomSide;
  Texture left, right, up, down;

  Player()
  {

  }
  Player(Texture &tin, int s, FloatRect b) : Sprite(tin)
  {
    speed = s;
    boundaries = Rect<int> (b);
    rightSide = boundaries.left+boundaries.width;
    bottomSide = boundaries.top+boundaries.height;

    float ratio = .666666666666666666666;
    Vector2f barLoc(b.left+b.width,0);
    Vector2f barSize(b.width/ratio,b.height*.05);
    health = new HealthBar(barLoc,barSize,5);

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
      std::cout << "left bound" << std::endl;
      move(boundaries.left,0);
    }
    else if (bounds.top+y<boundaries.top)
    {
      std::cout << "top bound" << std::endl;
      move(0,boundaries.top);
    }
    else if (bounds.left+bounds.width+x > rightSide)
    {
      std::cout << "right bound" << std::endl;
      move(rightSide-bounds.left-bounds.width,0);
    }
    else if (bounds.top+bounds.height+y > bottomSide)
    {
      std::cout << "bottom bound" << std::endl;
      move(bottomSide-bounds.top-bounds.height,0);
    }
    else
    {
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
    win.draw(*health);

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
