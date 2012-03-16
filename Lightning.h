/*
 * Lightning.h
 *
 *  Created on: Mar 16, 2012
 *      Author: emint
 */

#ifndef LIGHTNING_H_
#define LIGHTNING_H_

#include <vector>

#include "Turret.h"
#include "Shader.h"

#include "assimp/aiVector3D.h"

class Lightning : public Turret{
  public:
    Lightning(Shader* shader_);
    virtual ~Lightning();

    void render();

    //We override the time function here as
    //we are worrying about frames versus
    //time
    virtual void updateTime(float time);
  private:
    Shader* shader;

    aiVector3D color;

    //The location of each of the lightning paths
    std::vector<aiVector3D> vertices;
    std::vector<aiVector3D> colors;

    //Indices to render
    std::vector<unsigned> vertexIndex;

    //We try to render the same bolt multiple frames so it
    //does not just flicker
    unsigned int frameCount;

    //Sends information to shader
    void renderGenerated();
};

#endif /* LIGHTNING_H_ */
