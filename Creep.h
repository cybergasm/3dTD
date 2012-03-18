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
#include "TurretFactory.h"

#include "assimp/aiVector3D.h"
#include "SFML/Graphics/Image.hpp"

class Creep {
  public:
    Creep(Shader* creepShader, Maze* maze_, sf::Image* texture_,
        TurretFactory* turretFactory);
    virtual ~Creep();

    void render(float framerate);

    /**
     * The status of the creep
     */
    enum CreepStatus {
      CREEP_ALIVE, CREEP_DEAD, CREEP_ESCAPED
    };

    /**
     * Returns the status
     */
    CreepStatus getStatus();

    /**
     * Calculate damages and movement
     */
    void update(float framerate);
  private:
    //We need a maze to get current world 2information
    Maze* maze;

    //We need turret factory to query damage information
    TurretFactory* turretFactory;

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

    //The image with which to texture creep
    sf::Image* texture;

    //Vertex and normals
    std::vector<aiVector3D> colors;
    std::vector<aiVector3D> normals;
    std::vector<aiVector3D> texCoords;

    //The status of this creep
    CreepStatus status;

    //Creep health and original health
    float originalHealth;
    float health;

    /**
     * Updates location
     */
    void updatePosition(float framerate);

    /**
     * Updates health
     */
    void updateHealth(float framerate);
};

#endif /* CREEP_H_ */
