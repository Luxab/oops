//  Created by Duncan Klug on 4/6/17.
//	Lab 02 Spring 2017
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Button : public RectangleShape
{
  int x,y,bSize;
  int bSpacing = 10; //Space between text and edge of button

  Color bColDef; //default Color
  Color bColor;
  Font bFont;
  Text bText;
  int bWidth, bHeight; //width and height of the background.
  FloatRect textRegion;

public:

  Button(int xin, int yin, Font fin, String tin, int bsin) : RectangleShape(Vector2f(xin,yin))
  {
    x = xin;
    y = yin;
    bSize = bsin;
    bSpacing = 10+bSize/10;

    bColDef = Color(80,80,80);
    bColor = bColDef;
    bFont = fin;
    bText = Text(tin,bFont,bSize);
    textRegion = bText.getLocalBounds();
    bWidth = textRegion.width+bSpacing*2;
    bHeight = textRegion.height+bSpacing*2;

    setSize(Vector2f(bWidth,bHeight));
    setPosition(x-bSpacing,y);
    setFillColor(bColor);
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
    setPosition(x-bSpacing,y);
    bText.setPosition(x,y);
  }

  void setY(int yin)
  {
    y = yin;
    setPosition(x-bSpacing,y);
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
    int glowVal = 220;

    int asyGlow;
    if (contains(mx,my) && bColor.g<glowVal)
    {
      asyGlow = 20*bColor.g/bColDef.g;
      if (bColor.g+asyGlow>glowVal)
      {
        bColor.g = glowVal;
      }
      else
      {
        bColor.g+=asyGlow;
      }
    }
    else if (!contains(mx,my) && bColor.g>bColDef.g)
    {
      asyGlow = 20*bColor.g/bColDef.g;
      if (bColor.g-asyGlow<bColDef.g)
      {
        bColor.g = bColDef.g;
      }
      else
      {
        bColor.g-=asyGlow;
      }
    }
    setFillColor(bColor);
  }

  void draw(RenderWindow &window)
  {
    window.draw(*this);
    window.draw(bText);
  }

};

#endif
