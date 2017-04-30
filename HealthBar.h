//  Created by Duncan Klug on 4/18/17.
//	Lab 02 Spring 2017
#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class MovingHealthBar : public RectangleShape
{
  int maxHealth;
  int health;
  int x,y,bSize;
  int bWidth, bHeight; //width and height of the background.

public:

  MovingHealthBar (Vector2f location = Vector2f(0,0), Vector2f size = Vector2f(100,30), int mh=5) : RectangleShape(size)
  {
    maxHealth = mh;
    health = maxHealth;
    setPosition(location.x,location.y);
    setFillColor(Color(234,0,0)); // red
  }
  ~MovingHealthBar()
  {

  }

  void takeDamage(RenderWindow &window, int damageAmount)
  {
    health-= damageAmount;

    if (health < 0)
      health = 0;

    setSize(Vector2f(getSize().x*((float)health/(float)maxHealth),getSize().y));
  }

  int getCurrentHealth()
  {
    return health;
  }

  void setMaxHealth(int h)
  {
    maxHealth = h;
    health = h;
  }
};

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
    setFillColor(Color(234,0,0)); // red
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

  void takeDamage(RenderWindow &window, int damageAmount)
  {
    health-= damageAmount;
    setSize(Vector2f(getSize().x*((float)health/(float)maxHealth),getSize().y));
  }

  int getCurrentHealth()
  {
    return health;
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
#endif
