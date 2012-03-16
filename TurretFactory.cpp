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
  fireWheel = new FireWheel(particleSystemShader);

}

TurretFactory::~TurretFactory() {
  // TODO Auto-generated destructor stub
}

Turret* TurretFactory::getTurret(TurretType type) {
  switch(type) {
    case FIRE_WHEEL:
      return fireWheel;
      break;
    default:
      cerr << "Cannot find turret of specified type."<<endl;
      return NULL;
      break;
  }
}

void TurretFactory::updateTime(float framerate) {
  fireWheel->updateTime(10*framerate);
}
