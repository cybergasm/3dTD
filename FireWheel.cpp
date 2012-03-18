/*
 * FireWheel.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#include "FireWheel.h"

#include <iostream>

using namespace std;

FireWheel::FireWheel(Shader* particleSystemShader) :
  numParticles(8000), duration(40), numSpokes(8),
      particles(numParticles, duration) {
  initParticles();

  particles.shaderIs(particleSystemShader);
}

FireWheel::~FireWheel() {
  // TODO Auto-generated destructor stub
}

void FireWheel::render() {
  particles.renderAtTime(timeStep);
}

float FireWheel::damage(int numCreeps) {
  return 3.0f;
}
void FireWheel::initParticles() {
  aiVector3D vel;
  uint lifespan;
  aiVector3D color;
  aiVector3D pos;
  for (unsigned int spoke = 0; spoke < numSpokes; spoke++) {
    aiVector3D spokeVector = getSpokeVector(spoke);
    for (unsigned int particle = 0; particle < (numParticles/numSpokes); particle++) {
      vel.x = vel.z = 0.0f;
      vel.y = (rand() % 10 + 1) / 1000.0f;
      lifespan = (rand() % duration + duration * .9f);
      pos.x = spokeVector.x/5 * (particle) + ((rand()%30)/1000.0f);
      pos.z = spokeVector.z/5 * (particle) + ((rand()%30)/1000.0f);
      color = getParticleColor();
      particles.addParticle(pos, vel, aiVector3D(0,-vel.y/(lifespan),0), color, lifespan);
    }
  }
}

aiVector3D FireWheel::getParticleColor() {
  int color = rand() % 4;
  aiVector3D colorVals;
  switch (color) {
    case 0:
      colorVals.x = 253.f / 255.f;
      colorVals.y = 14.f / 255.f;
      colorVals.z = 23.f / 255.f;
      break;
    case 1:
      colorVals.x = 226.f / 255.f;
      colorVals.y = 88.f / 255.f;
      colorVals.z = 34.f / 255.f;
      break;
    case 2:
      colorVals.x = 255.f / 255.f;
      colorVals.y = 255.f / 255.f;
      colorVals.z = 0.f / 255.f;
      break;
    case 3:
      colorVals.x = 255.f / 255.f;
      colorVals.y = 139.f / 255.f;
      colorVals.z = 15.f / 255.f;
      break;
  }
  return colorVals;
}

aiVector3D FireWheel::getSpokeVector(unsigned int spoke) {
  switch(spoke) {
    case 0:
      return aiVector3D(0.0, 0.0, -.001);
    case 1:
      return aiVector3D(.001, 0.0, -.001);
    case 2:
      return aiVector3D(.001, 0.0, 0.0);
    case 3:
      return aiVector3D(.001, 0.0, .001);
    case 4:
      return aiVector3D(0.0, 0.0, .001);
    case 5:
      return aiVector3D(-.001, 0.0, .001);
    case 6:
      return aiVector3D(-.001, 0.0, 0.0);
    case 7:
      return aiVector3D(-.001, 0.0, -.001);
  }
  return aiVector3D(0.0, 0.0, -.001);
}
