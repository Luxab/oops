//  Created by Duncan Klug on 3/16/17.
//  Partitioned into header files 3/19/17
//	Lab 02 Spring 2017

#include "game.h"

//Globals
int resW = 800;
int resH = 600;

int main()
{
  Game g(resW,resH);
  g.play();
  // Thread playThread(&TicTacToe::play,&t);
  // playThread.launch();
  // Thread exitThread(&TicTacToe::checkExit,&t);
  // exitThread.launch();
  return 1;
}
