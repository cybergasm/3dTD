/*
 * GravityRain.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: emint
 */

#include <iostream>

#include "GravityRain.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510582097494459230
#endif

GravityRain::GravityRain(Shader* psystem) :
  numParticles(20000), duration(10), particles(numParticles, duration) {
  initParticles();
  particles.shaderIs(psystem);
  particles.modeIs(GL_POINTS);
}

GravityRain::~GravityRain() {
  // TODO Auto-generated destructor stub
}

void GravityRain::render() {
  particles.renderAtTime(timeStep);
}

float GravityRain::damage(int numCreeps) {
  return 10.0f/(float) numCreeps;
}
void GravityRain::initParticles() {
  aiVector3D vel;
  uint lifespan;
  aiVector3D color;
  aiVector3D pos;
  aiVector3D accel;
  for (unsigned int particle = 0; particle < numParticles; particle++) {
    float angle = (particle * 2 * M_PI) / numParticles;
    lifespan = (rand() % duration + duration * .9f);
    vel.y = (-.5 + ((rand()%5)/10.0f))/((float)lifespan) ;
    vel.x = vel.z = 0.0f;
    //Tiles go up to .5, these go to 1 so we divide
    pos.x = accel.x = cos(angle) / (50*lifespan);
    pos.z = accel.z = -sin(angle) / (50*lifespan);
    accel.y = -vel.y/((float)10*lifespan);
    pos.y = .5;
    color = getParticleColor();
    particles.addParticle(pos, vel, accel, color, lifespan);
  }
}


aiVector3D GravityRain::getParticleColor() {
  int color = rand() % 4;
  aiVector3D colorVals;
  switch (color) {
    case 0:
      colorVals.x = 0.f / 255.f;
      colorVals.y = 0.f / 255.f;
      colorVals.z = 0.f / 255.f;
      break;
    case 1:
      colorVals.x = 204.f / 255.f;
      colorVals.y = 226.f / 255.f;
      colorVals.z = 237.f / 255.f;
      break;
    case 2:
      colorVals.x = 208.f / 255.f;
      colorVals.y = 215.f / 255.f;
      colorVals.z = 219.f / 255.f;
      break;
    case 3:
      colorVals.x = 141.f / 255.f;
      colorVals.y = 144.f / 255.f;
      colorVals.z = 145.f / 255.f;
      break;
  }
  return colorVals;
}
