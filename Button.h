//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
  int x,y,h,w,buttonSize;
  Color buttonColor;
  Font buttonFont;
  Text buttonText;

public:

  Button(int xin, int yin, Color colin, Font fin, String tin, int bsin) :
    (x(xin),y(yin),buttonSize(bsin))
  {
    buttonColor = colin;
    buttonFont = fin;
    buttonText = *(new text(buttonText,buttonFont,buttonSize));
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

    Text *drawText = new Text(buttonText,buttonFont,90);
    drawText->setPosition(25,50);
    window.draw(*drawText);
  }

};
