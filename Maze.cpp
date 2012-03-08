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
  mazeString(mazeString_), tileWidth(.5), tileDepth(.5), tile(tileWidth, tileDepth) {
  tileShader = new Shader("shaders/mazetile");

  if (!tileShader->loaded()) {
    cerr<<"ERROR LOADING TILE SHADER."<<endl;
    exit(-1);
  }
}

Maze::~Maze() {
  delete tileShader;
}

void Maze::render() {
  GL_CHECK(glUseProgram(tileShader->programID()));
  glPushMatrix();
  tile.render(tileShader->programID());
  glTranslatef(0, 0, tileDepth);
  tile.render(tileShader->programID());
  glPopMatrix();
  GL_CHECK(glUseProgram(0));
}
