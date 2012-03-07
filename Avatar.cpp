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
      numParticles(2000), particles(numParticles), aniTime(0), aniDuration(20) {
  sideVector.x = 1.0f;
  sideVector.y = 0.0f;
  sideVector.z = 0.0f;

  srand(time(NULL));

  indices.reserve(numParticles);
  positions.reserve(numParticles);

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
    indices.push_back(particle);
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

  aniTime += 10 * framerate;

  setParticleValues();

  glPopMatrix();
  glDisable(GL_BLEND);
  GL_CHECK(glUseProgram(0));
}

void Avatar::setParticleValues() {

  //Need to keep track of time passed
  GLint cTimePtr = glGetUniformLocation(shader->programID(), "currentTime");
  GL_CHECK(glUniform1f(cTimePtr, aniTime));

  //How long one lifecycle of all particles is
  GLint duration = glGetUniformLocation(shader->programID(), "duration");
  GL_CHECK(glUniform1f(duration, aniDuration));

  //How quickly they move, where they start, how long they live, and what they
  //look like
  GLint velocity = glGetAttribLocation(shader->programID(), "velocityIn");
  GLint positionIn = glGetAttribLocation(shader->programID(), "positionIn");
  GLint life = glGetAttribLocation(shader->programID(), "lifespan");
  GLint color = glGetAttribLocation(shader->programID(), "color");

  //Some mandatory checks
  if (color == -1) {
    cerr << "Error getting color handle." << endl;
  }
  if (duration == -1) {
    cerr << "Error getting duration handle." << endl;
  }
  if (cTimePtr == -1) {
    cerr << "Error getting currentTime pointer." << endl;
  }

  if (velocity == -1) {
    cerr << "Error getting velocity pointer." << endl;
  }

  if (positionIn == -1) {
    cerr << "Error getting positionIn pointer." << endl;
  }

  if (life == -1) {
    cerr << "Error getting lifespan pointer." << endl;
  }
  GL_CHECK(glEnableVertexAttribArray(velocity));
  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(life));
  GL_CHECK(glEnableVertexAttribArray(color));

  //Create a vector holding the position of the avatar
  //and set that to the origin of the particles
  aiVector3D curPosition(x, y, z);
  positions.assign(numParticles, curPosition);

  glPointSize(5);

  //pass in attributes
  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &positions[0]));
  GL_CHECK(glVertexAttribPointer(velocity, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          particles.particleVels()));
  GL_CHECK(glVertexAttribPointer(color, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          particles.particleColors()));
  GL_CHECK(glVertexAttribPointer(life, 1, GL_UNSIGNED_INT, 0, 0,
          particles.particleLifespans()));
  GL_CHECK(glDrawElements(GL_POINTS, numParticles, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(velocity));
  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(life));
  GL_CHECK(glDisableVertexAttribArray(color));
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
}
