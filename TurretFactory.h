/*
 * TurretFactory.h
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#ifndef TURRETFACTORY_H_
#define TURRETFACTORY_H_


#include "Turret.h"
#include "FireWheel.h"
#include "Lightning.h"
#include "Shader.h"

class TurretFactory {
  public:
    TurretFactory(Shader* particleSystemShader);
    virtual ~TurretFactory();

    enum TurretType {
      FIRE_WHEEL,
      LIGHTNING
    };

    /**
     * Returns the specified turret to be rendered
     *
     **/
    Turret* getTurret(TurretType type);

    /**
     * Update Turret time. This keeps all turrets running at the same time,
     * and allows us to save space by only having one instance of any turret.
     */
    void updateTime(float framerate);
  private:
    FireWheel* fireWheel;
    Lightning* lightning;
};

#endif /* TURRETFACTORY_H_ */
