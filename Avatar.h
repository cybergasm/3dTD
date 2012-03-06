/*
 * Avatar.h
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#ifndef AVATAR_H_
#define AVATAR_H_

#include "assimp/aiVector3D.h"

class Avatar {
  public:

    /**
     * Constructor requiring the x,y,z of the
     * avatar's center in world coordinates
     */
    Avatar(float x_, float y_, float z_);
    virtual ~Avatar();

    /**
     * Displays the avatar on screen. Currently is just a
     * box
     */
    void render();

    void updatePosition(float x_, float y_, float z_, float xAng_, float yAng_,
        aiVector3D sideVec);
  private:

    //Position in space
    float x, y, z, xAng, yAng;

    //Stores what the side vector of the camera is so that
    //we can rotate around it.
    aiVector3D sideVector;

    //half-width/height of this avatar
    float halfSideLen;
};

#endif /* AVATAR_H_ */
