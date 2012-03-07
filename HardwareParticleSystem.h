/*
 * HardwareParticle.h
 *
 *  A particle defining the information necessary to simulate
 *  movement entirely in shaders
 *  Created on: Mar 6, 2012
 *      Author: emint
 */

#ifndef HARDWAREPARTICLESYSTEM_H_
#define HARDWAREPARTICLESYSTEM_H_
#include "assimp/aiTypes.h"

class HardwareParticleSystem {
  public:
    HardwareParticleSystem(int numParticles_);
    virtual ~HardwareParticleSystem();

    /**
     * Adds a particle to the system
     */
    void addParticle(aiVector3D vel, aiVector3D color, uint lifespan_);

    /**
     * Accessors to each value
     */
    aiVector3D particleVel(int particleNum);
    aiVector3D particleColor(int particleNum);
    uint particleLifespan(int particleNum);

    /**
     * Accessor for all data
     */
    aiVector3D* particleVels();
    aiVector3D* particleColors();
    uint* particleLifespans();

  private:

    // Total number of particles we have
    int numParticleTotal;

    // Particles set so far
    int numParticlesSet;

    /**
     * The values that particles in our hardware particle system will need
     */
    //velocity
    aiVector3D* vels;
    //color
    aiVector3D* color;
    //how long it lives
    uint* lifespans;
};

#endif /* HARDWAREPARTICLE_H_ */
