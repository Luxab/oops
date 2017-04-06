//  Created by Duncan Klug on 3/16/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
  int x,y,h,w;
  Color buttonColor;
  Font buttonFont

public:

  Button(int xin, int yin, int hin, int win, Color colin, Font fin) :
    (x(xin),y(yin),h(hin),w(win))
  {
    buttonColor = colin;
    buttonFont = fin;
  }
  ~Button()
  {

  }

  bool contains(int mx, int my)
  {
    return (mx>x && mx<x+w && my>y && my<y+h);
  }

  void drawButton(RenderWindow &window)
  {
    RectangleShape r;
    r.setSize(Vector2f(780,110));
    r.setPosition(15,50);
    r.setFillColor(buttonColor);
    window.draw(r);
  }

};
