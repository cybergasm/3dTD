/*
 * GravityRain.h
 *
 *  Created on: Mar 16, 2012
 *      Author: emint
 */

#ifndef GRAVITYRAIN_H_
#define GRAVITYRAIN_H_

#include "Turret.h"
#include "HardwareParticleSystem.h"
#include "Shader.h"

#include "assimp/aiVector3D.h"

class GravityRain: public Turret {
  public:
    GravityRain(Shader* psystemShader);
    virtual ~GravityRain();
  private:
    /**
     * Typical particle parameters
     */
    unsigned int numParticles;
    unsigned int duration;
    HardwareParticleSystem particles;

    void render();
    /**
     * Sets up the gravity particles
     */
    void initParticles();


    /**
     * Defining some colors
     */
    aiVector3D getParticleColor();
};

#endif /* GRAVITYRAIN_H_ */
