/*
 * CreepManager.h
 *
 *  Created on: Mar 17, 2012
 *      Author: emint
 */

#ifndef CREEPMANAGER_H_
#define CREEPMANAGER_H_

#include <vector>

#include "Creep.h"
#include "Shader.h"
#include "Maze.h"

using namespace std;

class CreepManager {
  public:
    CreepManager(Maze* maze_);
    virtual ~CreepManager();

    /**
     * Takes care of spawning and killing
     * creeps if needed
     */
    void updateCreeps();

    /**
     * Moves the creep timer forwards
     */
    void updateTime(float framerate);

    /**
     * Renders the creeps
     */
    void renderCreeps(float framerate);
  private:
    /**
     * The current creeps
     */
    vector<Creep> creeps;

    /**
     * When the next creep is coming
     */
    float nextCreepTime;

    /**
     * Shader for creeps
     */
    Shader* creepShader;

    /**
     * Pointer to maze for the creeps
     */
    Maze* maze;
};

#endif /* CREEPMANAGER_H_ */
