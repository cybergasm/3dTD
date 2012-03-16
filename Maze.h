/*
 * Maze.h
 *
 *  Created on: Mar 8, 2012
 *      Author: emint
 */

#ifndef MAZE_H_
#define MAZE_H_

#include <string>

#include "MazeTile.h"
#include "TileData.h"
#include "TurretFactory.h"
#include "Shader.h"

using namespace std;

class Maze {
  public:
    Maze(string mazeString_, Shader* psystemShader );
    virtual ~Maze();

    /**
     * Draw the maze specified by string
     */
    void render(float framerate);

    /**
     * Sets how the maze looks
     */
    void mazeStringIs(string mazeString_);

    /**
     * Change which tile is selected
     */
    void selectedInc();
    void selectedDec();

    /**
     * Adds turret of specified type to tile
     */
    void addTurret(TurretFactory::TurretType type);
  private:
    //The string representing the type of maze to draw. This string can have
    //one of four letters:
    // f - place tile forward of last
    // l - place tile left of last
    // r - place tile right of last
    // u - place tile pointing up
    // d - place tile pointing down
    string mazeString;

    //tile dimensions and space between consecutive ones
    float tileWidth, tileDepth, tileSpacing;

    //The per-tile data
    vector<TileData> tileData;

    //We take a pointer to a particle system shader to
    //cut down on redundant shader
    Shader* particleSystemShader;

    //The turret factor
    TurretFactory turretFactory;

    //The maze tile we will be rendering in various positions
    MazeTile tile;

    //Shader that makes tiles look pretty
    Shader* tileShader;

    /**
     * Utility functions to help render tiles; each renders a tile
     * in the specified direction relative to the one before
     */
    void addTile(char tile, bool selected, unsigned int index);
    void renderTileForward(bool selected, unsigned int index);
    void renderTileLeft(bool selected, unsigned int index);
    void renderTileRight(bool selected, unsigned int index);
    void renderTileUp(bool selected, unsigned int index);
    void renderTileDown(bool selected, unsigned int index);

    /**
     * Helps keep count of tiles already rendered to position new tiles
     * appropriately. This is mainly for the up and down tiles as they have
     * to be rotated around the origin then transported back to the front
     */
    float numTilesForward;
    float numTilesLeft;
    float numTilesRight;
    float numTilesUp;
    float numTilesDown;

    //Global count of tiles for parsing
    int numTiles;

    /**
     * Keeps track of which tile the user is currently selecting
     */
    unsigned int selectedTile;

    /**
     * Parses the maze string adding new tile data if needed
     */
    void parseMazeString();

    /**
     * Renders turrets for given tile
     */
    void renderTurrets(unsigned int turret);
};

#endif /* MAZE_H_ */
