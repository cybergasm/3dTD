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

MazeTile::MazeTile(float width_, float depth_) :
  width(width_), depth(depth_) {
  vertices.reserve(8);
  indices.reserve(4 * 6);
  normals.reserve(4 * 6);
  colors.reserve(4 * 6);

  initVertices();
  initTopFace();
  initBottomFace();
  initLeftFace();
  initRightFace();
  initFrontFace();
  initBackFace();
}

MazeTile::~MazeTile() {
  // TODO Auto-generated destructor stub
}

void MazeTile::initVertices() {
  vertices.push_back(aiVector3D(-width / 2.0f, .0, -depth / 2.0f));
  vertices.push_back(aiVector3D(width / 2.0f, .0, -depth / 2.0f));
  vertices.push_back(aiVector3D(width / 2.0f, .0, depth / 2.0f));
  vertices.push_back(aiVector3D(-width / 2.0f, .0, depth / 2.0f));

  vertices.push_back(aiVector3D(-width / 2.0f, -.1, -depth / 2.0f));
  vertices.push_back(aiVector3D(width / 2.0f, -.1, -depth / 2.0f));
  vertices.push_back(aiVector3D(width / 2.0f, -.1, depth / 2.0f));
  vertices.push_back(aiVector3D(-width / 2.0f, -.1, depth / 2.0f));
}

void MazeTile::initTopFace() {

  aiVector3D normal(0, 1, 0);
  aiVector3D color(.4, .2, .6);
  for (unsigned int i = 0; i < 4; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initBottomFace() {
  aiVector3D normal(0, -1, 0);
  aiVector3D color(.2, .9, .6);

  for (unsigned int i = 4; i < 8; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initLeftFace() {
  //append the proper vertices to the rest of
  //the vertex vector
  vertices.push_back(vertices[0]);
  vertices.push_back(vertices[4]);
  vertices.push_back(vertices[7]);
  vertices.push_back(vertices[3]);

  aiVector3D normal(-1, 0, 0);
  aiVector3D color(.6, .3, .6);

  for (unsigned int i = 8; i < 12; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initRightFace() {
  vertices.push_back(vertices[1]);
  vertices.push_back(vertices[5]);
  vertices.push_back(vertices[6]);
  vertices.push_back(vertices[2]);

  aiVector3D normal(1, 0, 0);
  aiVector3D color(.4, .7, .9);

  for (unsigned int i = 12; i < 16; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initFrontFace() {
  vertices.push_back(vertices[3]);
  vertices.push_back(vertices[2]);
  vertices.push_back(vertices[6]);
  vertices.push_back(vertices[7]);

  aiVector3D normal(1, 0, 0);
  aiVector3D color(.5, .8, .2);

  for (unsigned int i = 16; i < 20; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initBackFace() {
  vertices.push_back(vertices[0]);
  vertices.push_back(vertices[1]);
  vertices.push_back(vertices[5]);
  vertices.push_back(vertices[4]);

  aiVector3D normal(1, 0, 0);
  aiVector3D color(.5, .8, .2);

  for (unsigned int i = 20; i < 24; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::render(int shaderId) {
  GLint positionIn = glGetAttribLocation(shaderId, "positionIn");
  GLint colorIn = glGetAttribLocation(shaderId, "colorIn");

  if (positionIn == -1) {
    cerr << "Error retrieving position in for maze tile." << endl;
  }

  if (colorIn == -1) {
    cerr << "Error retrieving color in for maze tile." << endl;
  }

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(colorIn));
  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &vertices[0]));
  GL_CHECK(glVertexAttribPointer(colorIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &colors[0]));
  GL_CHECK(glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
}

