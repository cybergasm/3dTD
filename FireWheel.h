/*
 * FireWheel.h
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#ifndef FIREWHEEL_H_
#define FIREWHEEL_H_

#include "Turret.h"
#include "HardwareParticleSystem.h"
#include "Shader.h"

#include "assimp/aiVector3D.h"


class FireWheel : public  Turret {
  public:
    FireWheel(Shader* particleSystemShader);
    virtual ~FireWheel();

    void render();
  private:
    /**
     * Typical particle parameters
     */
    unsigned int numParticles;
    unsigned int duration;

    /**
     * How many spokes on our wheel
     */
    unsigned int numSpokes;

    /**
     * The particle system
     */
    HardwareParticleSystem particles;

    /**
     * Sets up values for the particles
     */
    void initParticles();

    /**
     * Given the spoke of the wheel gives
     * the vector along which particles should be
     * positioned
     */
    aiVector3D getSpokeVector(unsigned int spoke);

    /**
     * Returns a particle color
     */
    aiVector3D getParticleColor();
};

#endif /* FIREWHEEL_H_ */
