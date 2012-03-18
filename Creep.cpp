/*
 * Creep.cpp
 *
 *  Created on: Mar 17, 2012
 *      Author: emint
 */

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

#include <stdio.h>
#include <iostream>

#include "Creep.h"

Creep::Creep(Shader* creepShader, Maze* maze_) :
  maze(maze_), currentTile(0), movementRate(.15), position(0.0f, .10f, 0.25f),
      distanceLeft(0.0f), shader(creepShader), width(.2), height(.2), status(CREEP_ALIVE) {
  // TODO Auto-generated constructor stub
  colors.push_back(aiVector3D(.2, 0.0, .7));
  colors.push_back(aiVector3D(.2, 0.0, .7));
  colors.push_back(aiVector3D(.2, 0.0, .7));
  colors.push_back(aiVector3D(.2, 0.0, .7));
}

Creep::~Creep() {
  // TODO Auto-generated destructor stub
}

Creep::CreepStatus Creep::getStatus() {
  return status;
}
void Creep::render(float framerate) {
  if (distanceLeft <= 0.0f) {
    if (move.directions.size() == 0) {
      move = maze->getMove(currentTile);

      //In case we reached the end
      if (currentTile != maze->getNumTiles() - 1) {
        currentTile++;
      } else {
        status = CREEP_DEAD;
        return;
      }
    }
    distanceLeft = move.distances.at(move.distances.size() - 1);
    move.distances.pop_back();
    direction = move.directions.at(move.directions.size() - 1);
    move.directions.pop_back();
  }

  float moveAmount = movementRate * framerate;
  position += direction * moveAmount;
  distanceLeft -= moveAmount;

  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);
  GL_CHECK(glUseProgram(shader->programID()));

  /**
   * Set the vertex positions relative to the current center
   */
  unsigned int vertexIndex[4] = {0, 1, 2, 3};
  aiVector3D vertices[4];

  vertices[0].x = position.x - width / 2.0f;
  vertices[0].y = position.y + height / 2.0f;

  vertices[1].x = position.x + width / 2.0f;
  vertices[1].y = position.y + height / 2.0f;

  vertices[2].x = position.x + width / 2.0f;
  vertices[2].y = position.y - height / 2.0f;

  vertices[3].x = position.x - width / 2.0f;
  vertices[3].y = position.y - height / 2.0f;

  vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = position.z;

  /**
   * Pass our stuff in
   */
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
          vertices));
  GL_CHECK(glVertexAttribPointer(colorId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &colors[0]));

  GL_CHECK(glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, vertexIndex));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(colorId));

  GL_CHECK(glUseProgram(oldId));
}
