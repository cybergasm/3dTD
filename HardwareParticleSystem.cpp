/*
 * HardwareParticle.cpp
 *
 *  Created on: Mar 6, 2012
 *      Author: emint
 */

#include "HardwareParticleSystem.h"

#include <iostream>
#include <stdio.h>

using namespace std;

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

HardwareParticleSystem::HardwareParticleSystem(int numParticles_, float aniDuration) :
  shader(NULL), numParticleTotal(numParticles_), numParticlesSet(0), aniTime(0), duration(aniDuration) {
  pos.reserve(numParticleTotal);
  indices.reserve(numParticles_);
  vels = new aiVector3D[numParticleTotal];
  accels = new aiVector3D[numParticleTotal];
  color = new aiVector3D[numParticleTotal];
  lifespans = new uint[numParticleTotal];
}

HardwareParticleSystem::~HardwareParticleSystem() {
  delete [] vels;
  delete [] accels;
  delete [] color;
  delete [] lifespans;
}

void HardwareParticleSystem::addParticle(aiVector3D vel, aiVector3D accel, aiVector3D color_, uint lifespan) {
  if (numParticlesSet == numParticleTotal) {
    cerr<<__FILE__<<":Trying to add "<<numParticlesSet<<" particles which is more than "<<numParticleTotal<<"."<<endl;
    return;
  }
  vels[numParticlesSet] = vel;
  accels[numParticlesSet] = accel;
  color[numParticlesSet] = color_;
  indices.push_back(numParticlesSet);
  lifespans[numParticlesSet++] = lifespan;
}

void HardwareParticleSystem::uniformLocationIs(aiVector3D pos_) {
  pos.assign(numParticlesSet, pos_);
}

void HardwareParticleSystem::shaderIs(Shader* shader_) {
  shader = shader_;
}

void HardwareParticleSystem::render(float framerate) {
  if (shader == NULL) {
    cerr << "No shader set. Not rendering particle system."<<endl;
    return;
  }
  aniTime += 10 * framerate;

  //Need to keep track of time passed
  GLint cTimePtr = glGetUniformLocation(shader->programID(), "currentTime");
  GL_CHECK(glUniform1f(cTimePtr, aniTime));

  //How long one lifecycle of all particles is
  GLint durationIn = glGetUniformLocation(shader->programID(), "duration");
  GL_CHECK(glUniform1f(durationIn, duration));

  //How quickly they move, where they start, how long they live, and what they
  //look like
  GLint velocity = glGetAttribLocation(shader->programID(), "velocityIn");
  GLint acceleration = glGetAttribLocation(shader->programID(), "accelerationIn");
  GLint positionIn = glGetAttribLocation(shader->programID(), "positionIn");
  GLint life = glGetAttribLocation(shader->programID(), "lifespan");
  GLint colorId = glGetAttribLocation(shader->programID(), "color");

  //Some mandatory checks
  if (colorId == -1) {
    cerr << "Error getting color handle." << endl;
  }
  if (durationIn == -1) {
    cerr << "Error getting duration handle." << endl;
  }
  if (cTimePtr == -1) {
    cerr << "Error getting currentTime pointer." << endl;
  }

  if (velocity == -1) {
    cerr << "Error getting velocity pointer." << endl;
  }

  if (acceleration == -1) {
    cerr << "Error getting acceleration pointer." << endl;
  }
  if (positionIn == -1) {
    cerr << "Error getting positionIn pointer." << endl;
  }

  if (life == -1) {
    cerr << "Error getting lifespan pointer." << endl;
  }

  //enable our values.
  GL_CHECK(glEnableVertexAttribArray(velocity));
  GL_CHECK(glEnableVertexAttribArray(acceleration));
  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(life));
  GL_CHECK(glEnableVertexAttribArray(colorId));

  //Create a vector holding the position of the avatar
  //and set that to the origin of the particles
  glPointSize(5);

  //pass in attributes
  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &pos[0]));
  GL_CHECK(glVertexAttribPointer(velocity, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          vels));
  GL_CHECK(glVertexAttribPointer(acceleration, 3, GL_FLOAT, 0, sizeof(aiVector3D),
            accels));
  GL_CHECK(glVertexAttribPointer(colorId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          color));
  GL_CHECK(glVertexAttribPointer(life, 1, GL_UNSIGNED_INT, 0, 0,
          lifespans));
  GL_CHECK(glDrawElements(GL_POINTS, numParticlesSet, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(velocity));
  GL_CHECK(glDisableVertexAttribArray(acceleration));
  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(life));
  GL_CHECK(glDisableVertexAttribArray(colorId));
}

aiVector3D HardwareParticleSystem::particleVel(int particleNum) {
  if (particleNum >= numParticlesSet) {
    cerr<<__FILE__<<":Trying to access "<<particleNum<<" which was not set."<<endl;
    return aiVector3D();
  }
  return vels[particleNum];
}

aiVector3D HardwareParticleSystem::particleColor(int particleNum) {
  if (particleNum >= numParticlesSet) {
    cerr<<__FILE__<<":Trying to access "<<particleNum<<" which was not set."<<endl;
    return aiVector3D();
  }
  return color[particleNum];
}

uint HardwareParticleSystem::particleLifespan(int particleNum) {
  if (particleNum >= numParticlesSet) {
    cerr<<__FILE__<<":Trying to access "<<particleNum<<" which was not set."<<endl;
    return 0;
  }
  return lifespans[particleNum];
}

aiVector3D* HardwareParticleSystem::particleVels() {
  return vels;
}

aiVector3D* HardwareParticleSystem::particleColors() {
  return color;
}

uint* HardwareParticleSystem::particleLifespans() {
  return lifespans;
}
