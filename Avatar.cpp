/*
 * Avatar.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#include "Avatar.h"
#include <GL/glu.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209
#endif

Avatar::Avatar(float x_, float y_, float z_) :
  x(x_), y(y_), z(z_) {
}

Avatar::~Avatar() {
}

void Avatar::render() {
  glColor4f(.1, .6, .4, 1);

  glBegin(GL_QUADS);
  glVertex3f(x - .1, y - .1, z);
  glVertex3f(x - .1, y + .1, z);
  glVertex3f(x + .1, y + .1, z);
  glVertex3f(x + .1, y - .1, z);
  glEnd();
}

void Avatar::updatePosition(float x_, float y_, float z_) {
  x = x_;
  y = y_;
  z = z_;
}

