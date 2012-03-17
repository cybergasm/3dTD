/*
 * Turret.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#include "Turret.h"

Turret::Turret() : timeStep(0){
  // TODO Auto-generated constructor stub

}

Turret::~Turret() {
  // TODO Auto-generated destructor stub
}


void Turret::updateTime(float time) {
  timeStep += time;
}
