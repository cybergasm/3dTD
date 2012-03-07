/*
 * HardwareParticle.cpp
 *
 *  Created on: Mar 6, 2012
 *      Author: emint
 */

#include "HardwareParticleSystem.h"

#include <iostream>
using namespace std;

HardwareParticleSystem::HardwareParticleSystem(int numParticles_) :
  numParticleTotal(numParticles_), numParticlesSet(0) {
  vels = new aiVector3D[numParticleTotal];
  color = new aiVector3D[numParticleTotal];
  lifespans = new uint[numParticleTotal];
}

HardwareParticleSystem::~HardwareParticleSystem() {
  delete [] vels;
  delete [] color;
  delete [] lifespans;
}

void HardwareParticleSystem::addParticle(aiVector3D vel, aiVector3D color_, uint lifespan) {
  if (numParticlesSet == numParticleTotal) {
    cerr<<__FILE__<<":Trying to add "<<numParticlesSet<<" particles which is more than "<<numParticleTotal<<"."<<endl;
    return;
  }
  vels[numParticlesSet] = vel;
  color[numParticlesSet] = color_;
  lifespans[numParticlesSet++] = lifespan;
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
