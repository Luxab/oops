//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
  int x,y,bSize;
  int bSpacing = 10; //Space between text and edge of button

  Color bColDef; //default Color
  Color bColor;
  Font bFont;
  Text bText;
  RectangleShape bBG; // Button Background
  int bWidth, bHeight; //width and height of the background.
  FloatRect textRegion;

public:

  Button(int xin, int yin, Font fin, String tin, int bsin)
  {
    x = xin;
    y = yin;
    bSize = bsin;
    bSpacing = 10+bSize/10;

    Color tempCol(80,80,80); //Default Color
    bColDef = tempCol;
    bColor = bColDef;
    bFont = fin;
    bText = *(new Text(tin,bFont,bSize));
    textRegion = bText.getLocalBounds();
    bWidth = textRegion.width+bSpacing*2;
    bHeight = textRegion.height+bSpacing*2;

    bBG.setSize(Vector2f(bWidth,bHeight));
    bBG.setPosition(x-bSpacing,y);
    bBG.setFillColor(bColor);
    bText.setPosition(x,y);
  }
  ~Button()
  {

  }

  bool contains(int mx, int my)
  {
    return (mx>x-bSpacing && mx<x+bWidth-bSpacing && my>y && my<y+bHeight);
  }

  void centerWidth(int width)
  {
    x = (width-textRegion.width)/2;
    bBG.setPosition(x-bSpacing,y);
    bText.setPosition(x,y);
  }

  void setY(int yin)
  {
    y = yin;
    bBG.setPosition(x-bSpacing,y);
    bText.setPosition(x,y);
  }

  int getWidth()
  {
    return bWidth;
  }

  int getHeight()
  {
    return bHeight;
  }

  void checkHover(int mx, int my)
  {
    //Makes the button glow green when the mouse is over it
    if (contains(mx,my) && bColor.g<200)
    {
      bColor.g+=10;
    }
    else if (!contains(mx,my) && bColor.g>bColDef.g)
    {
      bColor.g-=15;
    }
    bBG.setFillColor(bColor);
  }

  void draw(RenderWindow &window)
  {
    window.draw(bBG);
    window.draw(bText);
  }

};
