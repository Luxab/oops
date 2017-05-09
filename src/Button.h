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

    bColDef = Color(80,80,80,150);
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

  void setText(String s)
  {
    bText = Text(s,bFont,bSize);
    textRegion = bText.getLocalBounds();
    bWidth = textRegion.width+bSpacing*2;
    bHeight = textRegion.height+bSpacing*2;

    setSize(Vector2f(bWidth,bHeight));
    setPosition(x-bSpacing,y);
    bText.setPosition(x,y);
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
    int glowVal = 190;

    int asyGlow;
    if (contains(mx,my))
    {
      if (bColor.g<glowVal)
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
      if (bColor.a<255)
      {
        asyGlow = 20*bColor.a/bColDef.a;
        if (bColor.a+asyGlow>255)
        {
          bColor.a = 255;
        }
        else
        {
          bColor.a+=asyGlow;
        }
      }
    }
    else
    {
      if (bColor.g>bColDef.g)
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
      if (bColor.a>bColDef.a)
      {
        asyGlow = 20*bColor.a/bColDef.a;
        if (bColor.a-asyGlow<bColDef.a)
        {
          bColor.a = bColDef.a;
        }
        else
        {
          bColor.a-=asyGlow;
        }
      }
    }
    setFillColor(bColor);
  }

  void draw(RenderWindow &window)
  {
    window.draw(*this);
    window.draw(bText);
  }

  void setColor (Color newColor)
  {
    setFillColor(newColor);
  }

};

#endif
