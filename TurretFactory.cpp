/*
 * TurretFactory.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#include <iostream>

#include "TurretFactory.h"

using namespace std;

TurretFactory::TurretFactory(Shader* particleSystemShader) {
  Shader* simpleShader = new Shader("shaders/simple");
  if (!simpleShader->loaded()) {
    cerr << "Error loading simple shader."<<endl;
    cerr << simpleShader->errors() <<endl;
    exit(-1);
  }
  fireWheel = new FireWheel(particleSystemShader);
  lightning = new Lightning(simpleShader);
}

TurretFactory::~TurretFactory() {
  // TODO Auto-generated destructor stub
}

Turret* TurretFactory::getTurret(TurretType type) {
  switch(type) {
    case FIRE_WHEEL:
      return fireWheel;
      break;
    case LIGHTNING:
      return lightning;
      break;
    default:
      cerr << "Cannot find turret of specified type."<<endl;
      return NULL;
      break;
  }
}

void TurretFactory::updateTime(float framerate) {
  fireWheel->updateTime(10.0f*framerate);
}
