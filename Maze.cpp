/*
 * Maze.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: emint
 */

#include <iostream>
#include <stdio.h>

#include "Maze.h"

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

Maze::Maze(string mazeString_) :
  mazeString(mazeString_), tileWidth(.5), tileDepth(.5), tileSpacing(.05),
      tile(tileWidth, tileDepth), numTilesForward(0), numTilesLeft(0),
      numTilesRight(0), numTilesUp(0), numTilesDown(0) {
  tileShader = new Shader("shaders/mazetile");

  if (!tileShader->loaded()) {
    cerr << "ERROR LOADING TILE SHADER." << endl;
    cerr << tileShader->errors() << endl;
    exit(-1);
  }
}

Maze::~Maze() {
  delete tileShader;
}

void Maze::render() {
  GL_CHECK(glUseProgram(tileShader->programID()));

  numTilesLeft = numTilesRight = numTilesDown = numTilesUp = numTilesForward
      = 0;

  glPushMatrix();
  //render first tile
  tile.render(tileShader->programID());

  for (unsigned int i = 0; i < mazeString.length(); i++) {
    addTile(mazeString[i]);
  }
  /**glPushMatrix();
   glPushMatrix();
   tile.render(tileShader->programID());
   glTranslatef(0, 0, tileDepth);
   tile.render(tileShader->programID());
   glTranslatef(tileWidth, 0, 0);
   tile.render(tileShader->programID());
   glTranslatef(0, 0, tileDepth);
   tile.render(tileShader->programID());
   glPopMatrix();

   glPushMatrix();
   glTranslatef(tileWidth, tileWidth / 2, tileDepth * 2.5);
   glRotatef(90, 1, 0, 0);
   tile.render(tileShader->programID());
   glPopMatrix();

   glTranslatef(tileWidth, tileWidth, tileDepth * 3);
   tile.render(tileShader->programID());

   glPopMatrix();*/

  glPopMatrix();
  GL_CHECK(glUseProgram(0));
}

void Maze::addTile(char tile) {
  switch (tile) {
    case 'f':
      renderTileForward();
      break;
    case 'l':
      renderTileLeft();
      break;
    case 'r':
      renderTileRight();
      break;
    case 'u':
      renderTileUp();
      break;
    case 'd':
      renderTileDown();
      break;
  }
}

void Maze::renderTileForward() {
  numTilesForward++;
  //Add spacing, move forward, and render
  glTranslatef(0, 0, tileSpacing);
  glTranslatef(0, 0, tileDepth);
  tile.render(tileShader->programID());
}

void Maze::renderTileLeft() {
  numTilesLeft++;
  //Add spacing, move left, and render
  glTranslatef(-tileSpacing, 0, 0);
  glTranslatef(-tileWidth, 0, 0);
  tile.render(tileShader->programID());
}

void Maze::renderTileRight() {
  numTilesRight++;
  //Add spacing move right and render
  glTranslatef(tileSpacing, 0, 0);
  glTranslatef(tileWidth, 0, 0);
  tile.render(tileShader->programID());
}

void Maze::renderTileUp() {
  numTilesUp++;
  //Reset original matrix and put it back
  //on the stack
  glPopMatrix();
  glPushMatrix();

  float displacementLR = numTilesRight - numTilesLeft;
  //don't want to account for this feller entirely yet
  float displacementUD = numTilesUp - numTilesDown - .5;
  //we add half to get it to move to the end of the last tile
  float displacementForward = numTilesForward + .5;

  glTranslatef(displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing);

  glRotatef(-90, 1, 0, 0);
  tile.render(tileShader->programID());
  //reset original matrix
  glPopMatrix();
  glPushMatrix();

  //want to move the next tile entirely past us

  //Reset the number of displacementUD to be the number we have seen in the
  //two directions
  displacementUD = numTilesUp - numTilesDown;
  //Want to move forward only equal to the height of this block
  //which is fixed at .1; this is the .1;
  displacementForward = numTilesForward;
  //Move us to this tile
  glTranslatef(displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1);
}

void Maze::renderTileDown() {
  numTilesDown++;
  //Reset original matrix and put it back
  //on the stack
  glPopMatrix();
  glPushMatrix();

  float displacementLR = numTilesRight - numTilesLeft;
  //don't want to account for this feller entirely yet, but we also need to
  //account for the fact that this 90 degree roation places it at an
  //overlay of a tile height
  float displacementUD = numTilesUp - numTilesDown + .5 - .1 - tileSpacing;
  //we add half to get it to move to the end of the last tile; have to move
  //it a height extra since it rotates in as opposed to away
  float displacementForward = numTilesForward + .5 + .1 + tileSpacing;

  glTranslatef(displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing);

  glRotatef(90, 1, 0, 0);
  tile.render(tileShader->programID());
  //reset original matrix
  glPopMatrix();
  glPushMatrix();

  //want to move the next tile entirely past us

  //Reset the number of displacementUD to be the number we have seen in the
  //two directions
  displacementUD = numTilesUp - numTilesDown;
  //Want to move forward only equal to the height of this block
  //which is fixed at .1; this is the .1;
  displacementForward = numTilesForward;
  //Move us to this tile
  glTranslatef(displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1);
}
void Maze::mazeStringIs(string mazeString_) {
  mazeString = mazeString_;
}
