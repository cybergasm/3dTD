/*
 * TileData.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#include "TileData.h"

TileData::TileData() : numCreeps(0) {
  // TODO Auto-generated constructor stub

}

TileData::~TileData() {
  // TODO Auto-generated destructor stub
}

void TileData::addTurret(TurretFactory::TurretType type) {
  turrets.insert(type);
}

set<TurretFactory::TurretType> TileData::getTurrets() {
  return turrets;
}

void TileData::addCreep() {
  numCreeps++;
}

void TileData::removeCreep() {
  numCreeps--;
}

int TileData::getNumCreeps() {
  return numCreeps;
}
