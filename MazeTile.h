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
#include "SFML/Graphics/Image.hpp"

#include "Shader.h"

using namespace std;

class MazeTile {
  public:
    MazeTile(float width_, float depth_);
    virtual ~MazeTile();

    //Draw the tile
    void render(int shaderId, bool selected);
  private:

    //How long and wide the tile is
    float width, depth;

    //The opacity texture
    sf::Image opacityTex;

    //vertex information
    vector<aiVector3D> vertices;
    vector<aiVector3D> texCoords;
    vector<unsigned> indices;
    vector<aiVector3D> normals;
    vector<aiVector3D> colors;

    //Regular tile color
    aiVector3D color;

    aiVector3D selectedColor;

    /**
     * Intiiallizes the points for the cube
     */
    void initVertices();
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
