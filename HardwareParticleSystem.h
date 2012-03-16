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

#include <vector>

#include "assimp/aiTypes.h"
#include "Shader.h"

class HardwareParticleSystem {
  public:
    HardwareParticleSystem(int numParticles_, float duration);
    virtual ~HardwareParticleSystem();

    /**
     * Adds a particle to the system
     */
    void addParticle(aiVector3D vel, aiVector3D accel, aiVector3D color, uint lifespan_);

    /**
     * Adds a particle to the system with a specified starting position
     */
    void addParticle(aiVector3D pos, aiVector3D vel, aiVector3D accel, aiVector3D color, uint lifespan_);

    /**
     * Positioning the particles
     */
    //sets same origin for all particles
    void uniformLocationIs(aiVector3D pos);

    /**
     * Displaying the particle system
     */

    //Uses the shader set to display the particles. Makes a few assumptions
    //about the shader. It has active attributes:
    // velocityIn for how fast the particles are moving
    // positionIn for where particle starts
    // lifespan for how long the particle is active
    // color for the color fo the particle
    //
    //It has active uniforms:
    // current animation time
    // how long the animation should last before all particles loop back at
    //   least once
    void renderWithFramerate(float framerate);

    /**
     * Same as above but allows the caller to specify the current time
     */
    void renderAtTime(float time);

    //sets which shader to use (needs to be called before
    //anything can be rendered)
    void shaderIs(Shader* shader);

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
    //Shader to use for particles
    Shader* shader;

    // Total number of particles we have
    int numParticleTotal;

    // Particles set so far
    int numParticlesSet;

    //How long the system has been running (for animation)
    float aniTime;

    //How old the oldest particle can be (or, how often the animation
    //should have run before all particles reset at least once).
    float duration;
    /**
     * The values that particles in our hardware particle system will need
     */
    //position
    std::vector<aiVector3D> pos;
    //velocity
    aiVector3D* vels;
    //acceleration
    aiVector3D* accels;
    //color
    aiVector3D* color;
    //how long it lives
    uint* lifespans;
    //indices to draw
    std::vector<unsigned> indices;

    //renders particles at specified time
    void render(float time);
};

#endif /* HARDWAREPARTICLE_H_ */
