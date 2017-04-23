//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include <vector>

using namespace sf;

class HealthBar : public RectangleShape
{
  int maxHealth;
  int health;
  int x,y,bSize;
  int bWidth, bHeight; //width and height of the background.
  std::vector<RectangleShape*> outlines;

public:

  HealthBar(Vector2f location = Vector2f(0,0), Vector2f size = Vector2f(100,30), int mh=5) : RectangleShape(size)
  {
    maxHealth = mh;
    health = maxHealth;
    setPosition(location.x,location.y);
    setFillColor(Color(234,0,0));
    for (int i = 0; i<maxHealth; i++)
    {
      RectangleShape *r = new RectangleShape(Vector2f(size.x/maxHealth,size.y));
      r->setPosition(Vector2f(location.x+i*r->getSize().x,location.y));
      r->setFillColor(Color(0,0,0,0));
      r->setOutlineColor(Color(0,0,0));
      r->setOutlineThickness(2);
      outlines.push_back(r);
    }
  }
  ~HealthBar()
  {

  }

  void takeDamage(RenderWindow &window)
  {
    health--;
    setSize(Vector2f(getSize().x*((float)health/(float)maxHealth),getSize().y));
  }

  void draw(RenderWindow &win)
  {
    win.draw(*this);
    for (int i = 0; i<outlines.size(); i++)
    {
      win.draw(*outlines[i]);
    }
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
    weapon = new BBGun();
    weapon->boundingRect = boundaries;

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
    w->shoot(pos);
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

  void draw(RenderWindow &win)
  {
    tickMove();
    weapon->draw(win);
    health->draw(win);

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
