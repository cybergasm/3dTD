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

#include "FireWheel.h"

class TileData {
  public:
    TileData();
    virtual ~TileData();

  private:

    /**
     * Each tile can have some combination of the possible
     * turrets
     */
    FireWheel* fireWheel;
};

#endif /* TILEDATA_H_ */
