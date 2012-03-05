/*
 * Avatar.h
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#ifndef AVATAR_H_
#define AVATAR_H_

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

    void updatePosition(float x_, float y_, float z_);
  private:

    //Position in space
    float x, y, z;
};

#endif /* AVATAR_H_ */
