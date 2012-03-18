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
#include "MazeTile.h"
#include "Turret.h"

Creep::Creep(Shader* creepShader, Maze* maze_, sf::Image* texture_,
    TurretFactory* turretFactory_) :
  maze(maze_), turretFactory(turretFactory_), currentTile(0),
      movementRate(.15), position(0.0f, .10f, 0.25f), distanceLeft(0.0f),
      shader(creepShader), width(.2), height(.2), texture(texture_),
      status(CREEP_ALIVE), originalHealth(100), health(originalHealth) {

  for (int i = 0; i < 8; i++) {
    colors.push_back(aiVector3D(.2, 0.0, .7));
  }

  for (int i = 0; i < 8; i++) {
    normals.push_back(aiVector3D(1, 0, 0));
  }

  texCoords.push_back(aiVector3D(0, 0, 0));
  texCoords.push_back(aiVector3D(1, 0, 0));
  texCoords.push_back(aiVector3D(1, 1, 0));
  texCoords.push_back(aiVector3D(0, 1, 0));

  texCoords.push_back(aiVector3D(0, 0, 0));
  texCoords.push_back(aiVector3D(.1, 0, 0));
  texCoords.push_back(aiVector3D(.1, .1, 0));
  texCoords.push_back(aiVector3D(0, .1, 0));

}

Creep::~Creep() {
  // TODO Auto-generated destructor stub
}

Creep::CreepStatus Creep::getStatus() {
  return status;
}

void Creep::update(float framerate) {
  updatePosition(framerate);
  updateHealth(framerate);
}

void Creep::updateHealth(float framerate) {
  //current tile is actually one after we are graphically on
  TileData curTile = maze->getTileData(currentTile - 1);
  set<TurretFactory::TurretType> turrets = curTile.getTurrets();

  for (set<TurretFactory::TurretType>::iterator iter = turrets.begin(); iter
      != turrets.end(); ++iter) {
    float damage = turretFactory->getTurret(*iter)->damage(1) * framerate;
    health -= damage;
  }
  if (health <= 0.0f) {
    status = CREEP_DEAD;
  }
}

void Creep::updatePosition(float framerate) {
  if (distanceLeft <= 0.0f) {
    if (move.directions.size() == 0) {
      move = maze->getMove(currentTile);

      //In case we reached the end
      if (currentTile != maze->getNumTiles() - 1) {
        currentTile++;
      } else {
        status = CREEP_ESCAPED;
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
}

void Creep::render(float framerate) {

  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);
  GL_CHECK(glUseProgram(shader->programID()));

  /**
   * Set the vertex positions relative to the current center
   */
  unsigned int vertexIndex[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  aiVector3D vertices[8];

  //First four are creep vertices
  vertices[0].x = position.x - width / 2.0f;
  vertices[0].y = position.y + height / 2.0f;

  vertices[1].x = position.x + width / 2.0f;
  vertices[1].y = position.y + height / 2.0f;

  vertices[2].x = position.x + width / 2.0f;
  vertices[2].y = position.y - height / 2.0f;

  vertices[3].x = position.x - width / 2.0f;
  vertices[3].y = position.y - height / 2.0f;

  //Then four for the health bar
  vertices[4].x = position.x - width / 2.0f;
  vertices[4].y = position.y + height;

  vertices[5].x = position.x - (width / 2.0f) + width * (health / originalHealth);
  vertices[5].y = position.y + height;

  vertices[6].x = position.x - (width / 2.0f) + width * (health / originalHealth);
  vertices[6].y = position.y + height - .05f;

  vertices[7].x = position.x - width / 2.0f;
  vertices[7].y = position.y + height - .05f;


  for (int i = 0; i < 8; i++) {
    vertices[i].z = position.z;
  }

  /**
   * Pass our stuff in
   */
  GLint positionIn = glGetAttribLocation(shader->programID(), "positionIn");
  GLint colorId = glGetAttribLocation(shader->programID(), "colorIn");
  GLint normalIn = glGetAttribLocation(shader->programID(), "normalIn");
  GLint texCoordIn = glGetAttribLocation(shader->programID(), "texCoordIn");
  GLint opac = glGetUniformLocation(shader->programID(), "opacityMap");

  if (positionIn == -1) {
    cerr << "Error getting position handle in Lightning." << endl;
  }

  if (colorId == -1) {
    cerr << "Error getting color handle in Lightning." << endl;
  }

  if (normalIn == -1) {
    cerr << "Error retrieving normal in for creep." << endl;
  }

  if (opac == -1) {
    cerr << "Error retrieving opacity in for creep." << endl;
  }

  if (texCoordIn == -1) {
    cerr << "Error retrieving texCoordIn for creep." << endl;
  }

  glUniform1i(opac, 0);
  glActiveTexture(GL_TEXTURE0);
  texture->Bind();

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(colorId));
  GL_CHECK(glEnableVertexAttribArray(normalIn));
  GL_CHECK(glEnableVertexAttribArray(texCoordIn));

  GL_CHECK(glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          vertices));
  GL_CHECK(glVertexAttribPointer(colorId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &colors[0]));
  GL_CHECK(glVertexAttribPointer(normalIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &normals[0]));
  GL_CHECK(glVertexAttribPointer(texCoordIn, 2, GL_FLOAT, 0, sizeof(aiVector3D),
          &texCoords[0]));

  GL_CHECK(glDrawElements(GL_QUADS, 8, GL_UNSIGNED_INT, vertexIndex));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(colorId));
  GL_CHECK(glDisableVertexAttribArray(normalIn));
  GL_CHECK(glDisableVertexAttribArray(texCoordIn));

  GL_CHECK(glUseProgram(oldId));
}
