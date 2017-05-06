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
  int x,y;
  Vector2f bSize;
  int bWidth, bHeight; //width and height of the background.
  std::vector<RectangleShape*> outlines;

public:

  HealthBar(Vector2f location = Vector2f(0,0), Vector2f size = Vector2f(100,30), int mh=5) : RectangleShape(size)
  {
    maxHealth = mh;
    health = maxHealth;
    bSize = size;
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

  void takeDamage(int damageAmount)
  {
    health-= damageAmount;
    setSize(Vector2f(bSize.x*((float)health/(float)maxHealth),bSize.y));
  }

  void addHealth (float amt)
  {
    health += amt;

    // Prevent health from overrunning
    if (health > maxHealth)
        health = maxHealth;

    setSize(Vector2f(getSize().x*((float)health/(float)maxHealth),getSize().y));
    takeDamage(0);
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
