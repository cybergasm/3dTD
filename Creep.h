/*
 * Creep.h
 *
 *  Created on: Mar 17, 2012
 *      Author: emint
 */

#ifndef CREEP_H_
#define CREEP_H_

#include <vector>

#include "Maze.h"
#include "Shader.h"

#include "assimp/aiVector3D.h"

class Creep {
  public:
    Creep(Shader* creepShader, Maze* maze_);
    virtual ~Creep();

    void render(float framerate);

    /**
     * The status of the creep
     */
    enum CreepStatus {
      CREEP_ALIVE,
      CREEP_DEAD,
      CREEP_ESCAPED
    };

    /**
     * Returns the status
     */
    CreepStatus getStatus();
  private:
    //We need a maze to get current world 2information
    Maze* maze;

    //Keeps track of which tile this creep is on
    int currentTile;

    //How quickly they are moving
    float movementRate;

    //Where they are in world coordinates
    aiVector3D position;

    //The sequence of moves we have to perform
    MoveSequence move;

    //Vector the direction of which we are moving
    aiVector3D direction;

    //How much farther to go on the last tile
    float distanceLeft;

    //The shader to render the creep
    Shader* shader;

    //Creep dimensions
    float width, height;

    //Vertex colors
    std::vector<aiVector3D> colors;

    //The status of this creep
    CreepStatus status;
};

#endif /* CREEP_H_ */
