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
#include "TurretFactory.h"

#include "SFML/Graphics/Image.hpp"

using namespace std;

class CreepManager {
  public:
    CreepManager(Maze* maze_, TurretFactory* turretFactory_);
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

    /**
     * The texture with which to render creeps
     */
    sf::Image creepTexture;

    /**
     * Turret factory for creep construction
     */
    TurretFactory* turretFactory;
};

#endif /* CREEPMANAGER_H_ */
