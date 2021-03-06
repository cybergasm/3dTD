/*
 * Avatar.h
 *
 *  This class represents an avatar which is itself the emitter of a particle
 *  system. You can set its position as well as the shader. It will track time
 *  and pass it in to the shader assuming it will know how to transform the vertices
 *  given that each vertex is given a velocity attribute that represents initial state.
 *
 *  This avatar also performs motion blur on itself as it is moving.
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#ifndef AVATAR_H_
#define AVATAR_H_

#include "assimp/aiVector3D.h"
#include "Shader.h"
#include <vector>

#include "HardwareParticleSystem.h"

class Avatar {
  public:

    /**
     * Constructor requiring the x,y,z of the
     * avatar's center in world coordinates
     */
    Avatar();
    virtual ~Avatar();

    /**
     * Displays the avatar on screen. The avatar is
     * a particle system in this case. Needs framerate for
     * smooth animation
     */
    void render(float framerate);

    /**
     * Changes where the avatar is located. The last parameter corresponds
     * to the orientation and tells us what the vector projecting our of the
     * side is so that we can adjust the avatar according to the camera angles
     */
    void updatePosition(float x_, float y_, float z_, float xAng_, float yAng_,
        aiVector3D sideVec);

    /**
     * Sets the shader so that we can change how avatar is rendered if we
     * wish
     */
    void setShader(Shader* shader);
  private:

    //Position in space
    float x, y, z, xAng, yAng;

    //Stores what the side vector of the camera is so that
    //we can rotate around it.
    aiVector3D sideVector;

    //half-width/height of this avatar
    float halfSideLen;

    //dictates how many particles we have
    unsigned int numParticles;

    //Shader to draw the avatar.
    Shader* shader;

    //animation duration
    unsigned int aniDuration;

    //blur timer
    unsigned int blurTimer;

    //current blur buffer
    unsigned int blurBuffer;

    //number of buffers used
    unsigned int usedBuffers;

    //The particles of this avatar
    HardwareParticleSystem particles;


    //Creates particles with random initial values
    void initializeParticles();

    //generates a color out of a set for the particle
    aiVector3D getParticleColor();

};

#endif /* AVATAR_H_ */
