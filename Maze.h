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
    string mazeString;

    //tile dimensions
    float tileWidth, tileDepth;
    //The maze tile we will be rendering in various positions
    MazeTile tile;

    //Shader that makes tiles look pretty
    Shader* tileShader;
};

#endif /* MAZE_H_ */
