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
  x(x_), y(y_), z(z_), xAng(0.0f), yAng(0.0f), halfSideLen(.05) {

  sideVector.x = 1.0f;
  sideVector.y = 0.0f;
  sideVector.z = 0.0f;
}

Avatar::~Avatar() {
}

void Avatar::render() {
  glColor4f(.1, .6, .4, 1);
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(xAng * (180.0f / M_PI), 0, 1, 0);
  glTranslatef(-x, -y, -z);

  glBegin(GL_QUADS);
  glVertex3f(x - halfSideLen, y - halfSideLen, z);
  glVertex3f(x - halfSideLen, y + halfSideLen, z);
  glVertex3f(x + halfSideLen, y + halfSideLen, z);
  glVertex3f(x + halfSideLen, y - halfSideLen, z);
  glEnd();

  glPopMatrix();
}

void Avatar::updatePosition(float x_, float y_, float z_, float xAng_,
    float yAng_, aiVector3D sideVec) {
  x = x_;
  y = y_;
  z = z_;
  xAng = xAng_;
  yAng = yAng_;
  sideVector = sideVec;
}

