/*
 * Avatar.h
 *
 *  This class represents an avatar which is itself the emitter of a particle
 *  system. You can set its position as well as the shader. It will track time
 *  and pass it in to the shader assuming it will know how to transform the vertices
 *  given that each vertex is given a velocity attribute that represents initial state.
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
    Avatar(float x_, float y_, float z_);
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

    //The particles of this avatar
    HardwareParticleSystem particles;

    //Creates particles with random initial values
    void initializeParticles();

    //Vertex array indices to draw
    std::vector<unsigned> indices;

    //time in avatar animation
    float aniTime;

    //animation duration
    unsigned int aniDuration;
};

#endif /* AVATAR_H_ */
