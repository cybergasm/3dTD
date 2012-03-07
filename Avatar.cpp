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
  initializeParticles();
}

Avatar::~Avatar() {
}

void Avatar::initializeParticles() {
  aiVector3D vel;
  uint lifespan;
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
      vel.z *= -1;
    }
    cout << vel.x << " " << vel.y << " " << vel.z << " " << lifespan << endl;
    lifespan = (rand() % aniDuration + aniDuration*.8f);
    particles.addParticle(vel, lifespan);
    indices.push_back(particle);
  }
}

void Avatar::render(float framerate) {
  GL_CHECK(glUseProgram(shader->programID()));
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(xAng * (180.0f / M_PI), 0, 1, 0);
  glTranslatef(-x, -y, -z);

  GLint cTimePtr = glGetUniformLocation(shader->programID(), "currentTime");
  GL_CHECK(glUniform1f(cTimePtr, aniTime));
  aniTime += 10*framerate;

  GLint duration = glGetUniformLocation(shader->programID(), "duration");
  GL_CHECK(glUniform1f(duration, aniDuration));

  GLint velocity = glGetAttribLocation(shader->programID(), "velocityIn");
  GLint positionIn = glGetAttribLocation(shader->programID(), "positionIn");
  GLint life = glGetAttribLocation(shader->programID(), "lifespan");

  if (duration == -1) {
    cerr << "Error getting duration handle."<<endl;
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

  vector<aiVector3D> positions;
  positions.reserve(numParticles);
  aiVector3D curPosition(x, y, z);
  positions.assign(numParticles, curPosition);
  glPointSize(5);
  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &positions[0]));
  GL_CHECK(glVertexAttribPointer(velocity, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          particles.particleVels()));
  GL_CHECK(glVertexAttribPointer(life, 1, GL_UNSIGNED_INT, 0, 0,
          particles.particleLifespans()));
  GL_CHECK(glDrawElements(GL_POINTS, numParticles, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(velocity));
  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(life));
  glPopMatrix();

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
}
