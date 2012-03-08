/*
 * MazeTile.h
 *
 *  Created on: Mar 8, 2012
 *      Author: emint
 */

#ifndef MAZETILE_H_
#define MAZETILE_H_

#include <vector>

#include "assimp/aiVector3D.h"

#include "Shader.h"

using namespace std;

class MazeTile {
  public:
    MazeTile(float width_, float depth_);
    virtual ~MazeTile();

    //Draw the tile
    void render(int shaderId);
  private:

    //How long and wide the tile is
    float width, depth;

    //Pointer to the shader
    vector<aiVector3D> vertices;
    vector<unsigned> indices;
    vector<aiVector3D> normals;
    vector<aiVector3D> colors;

    /**
     * The following functions add the information for each face.
     */
    void initTopFace();
    void initBottomFace();
    void initLeftFace();
    void initRightFace();
    void initFrontFace();
    void initBackFace();
};

#endif /* MAZETILE_H_ */
