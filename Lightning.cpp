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
  shader(shader_), color(1, 1, .5), frameCount(0), paused(false) {
}

Lightning::~Lightning() {
  // TODO Auto-generated destructor stub
}

void Lightning::render() {
  if (frameCount == 0) {
    frameCount = rand() % 15 + 16;
    if (!paused) {
      paused = true;
      return;
    } else {
      paused = false;
    }
    vertices.clear();
    vertexIndex.clear();
    colors.clear();
    int indexCount = 0;

    for (int i = 0; i < 2; i++) {
      float chance = (rand() % 100) / 100.0f;
      float sign = (chance > .5 ? -1 : 1);
      float initX = sign * ((rand() % 10*i) / 100.0f);
      float lineWidth = (rand() % 5+1);
      glLineWidth(lineWidth);
      renderBolt(aiVector3D(initX, 1.0f, 0.0f), 1.0f, indexCount);
    }

    colors.assign(2 * vertices.size() + 1, color);
    renderGenerated();

  } else {
    if (!paused) {
      renderGenerated();
    }
  }
}

void Lightning::updateTime(float time) {
  if (frameCount != 0) {
    frameCount--;
  }
}

float Lightning::damage(int numCreeps) {
  return numCreeps*4.0f;
}
void Lightning::renderBolt(aiVector3D initPosition, float len,
    int& indexCount) {
  float height = initPosition.y;
  float xOff = initPosition.x;
  float zOff = initPosition.z;
  float chance = (rand() % 100) / 100.0f;
  aiVector3D leader(xOff, height, zOff);
  vertices.push_back(leader);
  vertexIndex.push_back(indexCount);
  indexCount++;
  while ((initPosition.y - height) < len) {
    chance = (rand() % 100) / 100.0f;
    //changing the direction possibly
    if (chance < .25f) {
      chance = (rand() % 100) / 100.0f;
      if (xOff < .25 && chance > .5) {
        xOff += .05;
      } else if (xOff > -.25 && chance < .5) {
        xOff -= .05;
      }
    } else if (chance >= .25f && chance < .50f) {
      chance = (rand() % 100) / 100.0f;
      if (zOff < .25 && chance > .5) {
        zOff += .05;
      } else if (zOff > -.25 && chance < .5) {
        zOff -= .05;
      }
    }

    height -= .05f;

    //We push once here to ensure we finish the line started by the
    //point before
    vertices.push_back(aiVector3D(xOff, height, zOff));
    vertexIndex.push_back(indexCount);
    indexCount++;

    if ((initPosition.y - height) < len) {
      chance = (rand() % 100) / 100.0f;
      if (chance < .25) {
        //we are branching so we want to call ourselves starting at this point to make
        //another smaller branch
        renderBolt(aiVector3D(xOff, height, zOff), (len - (initPosition.y - height)) / 2.0f,
            indexCount);
        //once we've rendered side branch have to place this point back
        //so that the following points of current branch connect to it
        vertices.push_back(aiVector3D(xOff, height, zOff));
        vertexIndex.push_back(indexCount);
        indexCount++;
      } else {
        //no add this point again so it starts the next line
        vertices.push_back(aiVector3D(xOff, height, zOff));
        vertexIndex.push_back(indexCount);
        indexCount++;
      }
    }

  }
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
