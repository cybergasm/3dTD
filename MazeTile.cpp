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
  texCoords.reserve(4 * 6);

  initVertices();
  initTopFace();
  initBottomFace();
  initLeftFace();
  initRightFace();
  initFrontFace();
  initBackFace();

  opacityTex.LoadFromFile("models/tile_opacity_texture2.jpg");
  glActiveTexture(GL_TEXTURE0);
  opacityTex.Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glGenerateMipmapEXT(GL_TEXTURE_2D);
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

  //initialize the texture coordinates
  for (int i = 0; i < 6; i++) {
    texCoords.push_back(aiVector3D(0, 0, 0));
    texCoords.push_back(aiVector3D(1, 0, 0));
    texCoords.push_back(aiVector3D(1, 1, 0));
    texCoords.push_back(aiVector3D(0, 1, 0));
  }
}

void MazeTile::initTopFace() {

  aiVector3D normal(0, 1, 0);
  aiVector3D color(.8, .7, .9);
  for (unsigned int i = 0; i < 4; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::initBottomFace() {
  aiVector3D normal(0, -1, 0);
  aiVector3D color(.8, .7, .9);

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
  aiVector3D color(.92, .92, .92);

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
  aiVector3D color(.92, .92, .92);

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
  aiVector3D color(.92, .92, .92);

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
  aiVector3D color(.92, .92, .92);

  for (unsigned int i = 20; i < 24; i++) {
    indices.push_back(i);
    normals.push_back(normal);
    colors.push_back(color);
  }
}

void MazeTile::render(int shaderId) {
  GLint positionIn = glGetAttribLocation(shaderId, "positionIn");
  GLint colorIn = glGetAttribLocation(shaderId, "colorIn");
  GLint normalIn = glGetAttribLocation(shaderId, "normalIn");
  GLint texCoordIn = glGetAttribLocation(shaderId, "texCoordIn");
  GLint opac = glGetUniformLocation(shaderId, "opacityMap");

  if (positionIn == -1) {
    cerr << "Error retrieving position in for maze tile." << endl;
  }

  if (colorIn == -1) {
    cerr << "Error retrieving color in for maze tile." << endl;
  }

  if (normalIn == -1) {
    cerr << "Error retrieving normal in for maze tile." << endl;
  }

  if (opac == -1) {
    cerr << "Error retrieving opacity in for maze tile." << endl;
  }

  if (texCoordIn == -1) {
    cerr << "Error retrieving texCoordIn for maze tile." << endl;
  }

  glUniform1i(opac, 0);
  glActiveTexture(GL_TEXTURE0);
  opacityTex.Bind();

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(colorIn));
  GL_CHECK(glEnableVertexAttribArray(normalIn));
  GL_CHECK(glEnableVertexAttribArray(texCoordIn));

  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &vertices[0]));
  GL_CHECK(glVertexAttribPointer(colorIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &colors[0]));
  GL_CHECK(glVertexAttribPointer(normalIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &normals[0]));
  GL_CHECK(glVertexAttribPointer(texCoordIn, 2, GL_FLOAT, 0, sizeof(aiVector3D),
          &texCoords[0]));

  GL_CHECK(glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, &indices[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(colorIn));
  GL_CHECK(glDisableVertexAttribArray(normalIn));
  GL_CHECK(glDisableVertexAttribArray(texCoordIn));
}

