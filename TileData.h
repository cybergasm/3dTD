/*
 * TileData.h
 *
 *  Class captures the important information
 *  about a tile, such as the turrets it has
 *  and the total number of dps it inflicts
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#ifndef TILEDATA_H_
#define TILEDATA_H_

#include <set>

#include "TurretFactory.h"


using namespace std;

class TileData {
  public:
    TileData();
    virtual ~TileData();

    /**
     * Insert turret type
     */
    void addTurret(TurretFactory::TurretType type);

    /**
     * Gets turrets
     */
    set<TurretFactory::TurretType> getTurrets();

    /**
     * Adds a creep to the tile
     */
    void addCreep();

    /**
     * Removes a creep from the tile
     */
    void removeCreep();

    /**
     * Tells how many creeps are on tile
     */
    int getNumCreeps();
  private:

    /**
     * Each tile can have some combination of the possible
     * turrets, stored as a set of types as we allow one
     * turret per square
     */
    set<TurretFactory::TurretType> turrets;

    /**
     * Count of creeps on tile
     */
    int numCreeps;

};

#endif /* TILEDATA_H_ */
