/*
 * Avatar.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#include "Avatar.h"
#include <GL/glew.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209
#endif

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

Avatar::Avatar(float x_, float y_, float z_) :
  x(x_), y(y_), z(z_), xAng(0.0f), yAng(0.0f), halfSideLen(.05),
      numParticles(2000), aniDuration(20), particles(numParticles, aniDuration) {
  sideVector.x = 1.0f;
  sideVector.y = 0.0f;
  sideVector.z = 0.0f;

  srand(time(NULL));

  initializeParticles();
}

Avatar::~Avatar() {
}

void Avatar::initializeParticles() {
  aiVector3D vel;
  uint lifespan;
  aiVector3D color;
  float flip;
  for (unsigned int particle = 0; particle < numParticles; particle++) {
    vel.x = (rand() % 10 + 1) / 1000.0f;
    flip = (rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.x *= -1;
    }
    vel.y = (rand() % 10 + 1) / 1000.0f;
    flip = (rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.y *= -1;
    }
    vel.z = (rand() % 10 + 1) / 1000.0f;
    flip = -(rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.z *= 1;
    }
    cout << vel.x << " " << vel.y << " " << vel.z << " " << lifespan << endl;
    lifespan = (rand() % aniDuration + aniDuration * .8f);
    color = getParticleColor();
    cout << color.x << " " << color.y << " " << color.z << endl;
    particles.addParticle(vel, color, lifespan);
  }
}

aiVector3D Avatar::getParticleColor() {
  int color = rand() % 4;
  aiVector3D colorVals;
  switch (color) {
    case 0:
      colorVals.x = 204.f / 255.f;
      colorVals.y = 234.f / 255.f;
      colorVals.z = 252.f / 255.f;
      break;
    case 1:
      colorVals.x = 61.f / 255.f;
      colorVals.y = 91.f / 255.f;
      colorVals.z = 110.f / 255.f;
      break;
    case 2:
      colorVals.x = 213.f / 255.f;
      colorVals.y = 227.f / 255.f;
      colorVals.z = 235.f / 255.f;
      break;
    case 3:
      colorVals.x = 107.f / 255.f;
      colorVals.y = 159.f / 255.f;
      colorVals.z = 191.f / 255.f;
      break;
  }
  return colorVals;
}

void Avatar::render(float framerate) {
  GL_CHECK(glUseProgram(shader->programID()));
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(xAng * (180.0f / M_PI), 0, 1, 0);
  glTranslatef(-x, -y, -z);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //Create a vector holding the position of the avatar
  //and set that to the origin of the particles
  aiVector3D curPosition(x, y, z);
  particles.uniformLocationIs(curPosition);
  particles.render(framerate);

  glPopMatrix();
  glDisable(GL_BLEND);
  GL_CHECK(glUseProgram(0));
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

void Avatar::setShader(Shader* shader_) {
  shader = shader_;
  particles.shaderIs(shader);
}
