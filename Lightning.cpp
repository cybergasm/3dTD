/*
 * Lightning.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: emint
 */

#include "Lightning.h"

#include <stdio.h>
#include <iostream>

using namespace std;

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

Lightning::Lightning(Shader* shader_) :
  shader(shader_), color(1, 1, 0), frameCount(0) {
}

Lightning::~Lightning() {
  // TODO Auto-generated destructor stub
}

void Lightning::render() {
  if (frameCount == 0) {
    vertices.clear();
    vertexIndex.clear();
    colors.clear();
    float height = 1.0f;
    float xOff = 0.0f;
    float zOff = 0.0f;
    aiVector3D leader(xOff, height, zOff);
    int indexCount = 0;
    vertices.push_back(leader);
    vertexIndex.push_back(indexCount);
    indexCount++;
    float chance = 0.0f;
    while (height > 0) {
      chance = (rand() % 100) / 100.0f;
      if (chance < .25f) {
        xOff += .01;
      } else if (chance >= .25f && chance < .50f) {
        zOff += .01;
      }
      height -= .1f;

      //We push twice to ensure that this index is the end of the last line
      //and beginning of next
      vertices.push_back(aiVector3D(xOff, height, zOff));
      vertexIndex.push_back(indexCount);
      indexCount++;
      vertices.push_back(aiVector3D(xOff, height, zOff));
      vertexIndex.push_back(indexCount);
      indexCount++;
    }
    colors.assign(2*vertices.size()+1, color);
    frameCount = rand() % 4 + 6;
    renderGenerated();
  } else {
    renderGenerated();
  }
}

void Lightning::updateTime(float time) {
  cout<<frameCount<<endl;
  frameCount--;
}
void Lightning::renderGenerated() {
  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);
  GL_CHECK(glUseProgram(shader->programID()));

  GLint positionIn = glGetAttribLocation(shader->programID(), "positionIn");
  GLint colorId = glGetAttribLocation(shader->programID(), "colorIn");

  if (positionIn == -1) {
    cerr << "Error getting position handle in Lightning." << endl;
  }

  if (colorId == -1) {
    cerr << "Error getting color handle in Lightning." << endl;
  }

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(colorId));

  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &vertices[0]));
  GL_CHECK(glVertexAttribPointer(colorId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &colors[0]));

  GL_CHECK(glDrawElements(GL_LINES, vertexIndex.size(), GL_UNSIGNED_INT, &vertexIndex[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(colorId));

  GL_CHECK(glUseProgram(oldId));
}
