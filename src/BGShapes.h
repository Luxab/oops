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

#ifndef BGSHAPES_H
#define BGSHAPES_H

using namespace sf;

typedef enum SHAPE_COLOR
{
  SHAPES_BLUE,
  SHAPES_GREEN,
  SHAPES_RED
} SHAPE_COLOR;

class BGShapes
{
  RenderWindow *window;
  RectangleShape *background;

  Clock backgroundCirclesClock;                     // Timer to spawn new background circles
  std::vector<CircleShape*> backgroundCircles;      // Vector to hold all background circles
  double backgroundCirclesCooldown = 400;           // Cooldown for spawning new circles
  double backgroundCirclesScaleAmt = 5;             // Rate at which circles scale
  Vector2f spawnLocation;                           // Location of where circles spawn
  SHAPE_COLOR shapeColor;

public:
  BGShapes(RectangleShape &b, RenderWindow &win, Vector2f spawnLoc, SHAPE_COLOR sc)
  {
    window = &win;
    background = &b;
    shapeColor = sc;
    spawnLocation = spawnLoc;

    background->setFillColor(Color(0,0,0));
  }
  ~BGShapes()
  {

  }

//---------------------------------------------------------------------------------//

  void draw()
  {
    FloatRect bgDim = background->getGlobalBounds();

    // Add a new circle every few hundred milliseconds
    if (backgroundCirclesClock.getElapsedTime().asMilliseconds() > backgroundCirclesCooldown)
    {
      // Generate new circle
      //

      // Random vars
      std::random_device rd;
      std::mt19937 rng(rd());

      // Random angle
      std::uniform_real_distribution<double> uni(-M_PI/4,M_PI/4);
      auto random_angle = uni(rng);


      int r_val,r_val_max;
      int g_val,g_val_max;
      int b_val,b_val_max;

      switch (shapeColor) {
        case SHAPES_RED:
            r_val = 100;r_val_max = 255;
            g_val = 0;g_val_max = 55;
            b_val = 0;b_val_max = 55;
          break;
        case SHAPES_GREEN:
            r_val = 0;r_val_max = 55;
            g_val = 100;g_val_max = 255;
            b_val = 0;b_val_max = 55;
          break;
        case SHAPES_BLUE:
            r_val = 0;r_val_max = 55;
            g_val = 0;g_val_max = 55;
            b_val = 100;b_val_max = 255;
          break;
        default:
          break;
      }

      std::uniform_int_distribution<int> uni_color_r(r_val,r_val_max);
      std::uniform_int_distribution<int> uni_color_g(g_val,g_val_max);
      std::uniform_int_distribution<int> uni_color_b(b_val,b_val_max);

      auto random_r = uni_color_r(rng);
      auto random_g = uni_color_g(rng);
      auto random_b = uni_color_b(rng);

      CircleShape *c = new CircleShape(0,30);
      c->setPosition(spawnLocation);
      c->setFillColor(Color(random_r, random_g, random_b));
      backgroundCircles.push_back(c);

      // Reset timer
      backgroundCirclesClock.restart();
    }

    // Grow every circle in the vector
    for (CircleShape *c : backgroundCircles)
    {
      // Draw each circle
      window->draw(*c);
      c->setRadius(c->getRadius() + backgroundCirclesScaleAmt);
      //c->setPosition(bgDim.width/2 - c->getRadius(),bgDim.height/2 - c->getRadius());
      c->setPosition(spawnLocation.x - c->getRadius(),spawnLocation.y - c->getRadius());
    }

    for (int i = 0; i < backgroundCircles.size();)
    {
      CircleShape *c = backgroundCircles.at(i);

      // Delete circles that are too large
      if (c->getRadius() > bgDim.width * 1.5)
        backgroundCircles.erase(backgroundCircles.begin() + i);
      else
        i++;
    }
  }
};

#endif
