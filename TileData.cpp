/*
 * TileData.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#include "TileData.h"

TileData::TileData() {
  // TODO Auto-generated constructor stub

}

TileData::~TileData() {
  // TODO Auto-generated destructor stub
}

void TileData::addTurret(TurretFactory::TurretType type) {
  turrets.insert(type);
}
