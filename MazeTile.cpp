/*
 * MazeTile.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: emint
 */

#include <GL/glew.h>
#include <iostream>
#include <stdio.h>

#include "MazeTile.h"

using namespace std;

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

MazeTile::MazeTile(float width_, float depth_) : width(width_), depth(depth_) {
  vertices.reserve(4*6);
  indices.reserve(4*6);
  normals.reserve(4*6);
  colors.reserve(4*6);

  initTopFace();
}

void MazeTile::initTopFace() {
  vertices.push_back(aiVector3D(-width/2.0f, .1, -depth/2.0f));
  vertices.push_back(aiVector3D(width/2.0f, .1, -depth/2.0f));
  vertices.push_back(aiVector3D(width/2.0f, .1, depth/2.0f));
  vertices.push_back(aiVector3D(-width/2.0f, .1, depth/2.0f));

  aiVector3D normal(0, 1, 0);
  aiVector3D color(.4, .2, .6);
  for (unsigned int i=0; i<4; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}


void MazeTile::render (int shaderId) {
  GLint positionIn = glGetAttribLocation(shaderId, "positionIn");

  if (positionIn == -1) {
    cerr<<"Error retrieving position in for maze tile."<<endl;
  }

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
            &vertices[0]));
  GL_CHECK(glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
}
MazeTile::~MazeTile() {
  // TODO Auto-generated destructor stub
}
