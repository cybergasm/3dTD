/*
 * Turret.h
 * The turret interface
 *
 *  Created on: Mar 15, 2012
 *      Author: emint
 */

#ifndef TURRET_H_
#define TURRET_H_

class Turret {
  public:
    Turret();
    virtual ~Turret();

    /**
     * The pure virtual method that needs
     * to be rendered by a turret type
     */
    virtual void render() = 0;

    /**
     * Moves the animation timestep for the turret forward
     */
    virtual void updateTime(float time);

    /**
     * Returns how much damage this turret does based on
     * how many creeps there are on its tile. The damage is
     * given in damage per time
     */
    virtual float damage(int numCreeps) = 0;
  protected:
    float timeStep;
};

#endif /* TURRET_H_ */
