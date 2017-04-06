//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
  int x,y,bSize;
  int bSpacing = 10; //Space between text and edge of button
  Color bColor;
  Font bFont;
  Text bText;
  RectangleShape bBG; // Button Background
  FloatRect textRegion;

public:

  Button(int xin, int yin, Color colin, Font fin, String tin, int bsin)
  {
    x = xin;
    y = yin;
    bSize = bsin;
    bSpacing = 10+bsin/10;

    bColor = colin;
    bFont = fin;
    bText = *(new Text(tin,bFont,bSize));
    textRegion = bText.getLocalBounds();
  }
  ~Button()
  {

  }

  bool contains(int mx, int my)
  {
    return (textRegion.contains(mx,my));
  }

  void centerWidth(int width)
  {
    x = (width-textRegion.width)/2;
  }

  void drawButton(RenderWindow &window)
  {
    bBG.setSize(Vector2f(textRegion.width+bSpacing*2,textRegion.height+bSpacing*2));
    bBG.setPosition(x-bSpacing,y-bSpacing);
    bBG.setFillColor(bColor);
    window.draw(bBG);

    bText.setPosition(x,y);
    window.draw(bText);
  }

};
