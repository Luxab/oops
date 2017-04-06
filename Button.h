//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
  int x,y,buttonSize;
  int buttonSpacing = 10; //Space between text and edge of button
  Color buttonColor;
  Font buttonFont;
  Text buttonText;
  RectangleShape buttonBackground;
  FloatRect textRegion;

public:

  Button(int xin, int yin, Color colin, Font fin, String tin, int bsin)
  {
    x = xin;
    y = yin;
    buttonSize = bsin;

    buttonColor = colin;
    buttonFont = fin;
    buttonText = *(new Text(tin,buttonFont,buttonSize));
    textRegion = buttonText.getLocalBounds();
  }
  ~Button()
  {

  }

  bool contains(int mx, int my)
  {
    return (textRegion.contains(mx,my));
  }

  void drawButton(RenderWindow &window)
  {
    RectangleShape r;
    r.setSize(Vector2f(textRegion.width+buttonSpacing,textRegion.height+buttonSpacing));
    r.setPosition(x,y);
    r.setFillColor(buttonColor);
    window.draw(r);

    buttonText.setPosition(x,y);
    window.draw(buttonText);
  }

};
