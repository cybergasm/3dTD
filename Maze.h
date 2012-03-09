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
#include "Shader.h"

using namespace std;

class Maze {
  public:
    Maze(string mazeString_);
    virtual ~Maze();

    /**
     * Draw the maze specified by string
     */
    void render();
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
    //The maze tile we will be rendering in various positions
    MazeTile tile;

    //Shader that makes tiles look pretty
    Shader* tileShader;

    /**
     * Sets how the maze looks
     */
    void mazeStringIs(string mazeString_);
    /**
     * Utility functions to help render tiles; each renders a tile
     * in the specified direction relative to the one before
     */
    void addTile(char tile);
    void renderTileForward();
    void renderTileLeft();
    void renderTileRight();
    void renderTileUp();
    void renderTileDown();

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
};

#endif /* MAZE_H_ */
