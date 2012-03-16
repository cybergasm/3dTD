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

  protected:
    float timeStep;
};

#endif /* TURRET_H_ */
