/*
 * Maze.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: emint
 */

#include <iostream>
#include <stdio.h>

#include "Maze.h"

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

Maze::Maze(string mazeString_, Shader* psystemShader) :
  mazeString(mazeString_), tileWidth(.5), tileDepth(.5), tileSpacing(.05),
      particleSystemShader(psystemShader), turretFactory(particleSystemShader),
      tile(tileWidth, tileDepth), numTilesForward(0), numTilesLeft(0),
      numTilesRight(0), numTilesUp(0), numTilesDown(0), numUpDownRuns(0),
      numTiles(0), selectedTile(0) {
  tileShader = new Shader("shaders/mazetile");

  if (!tileShader->loaded()) {
    cerr << "ERROR LOADING TILE SHADER." << endl;
    cerr << tileShader->errors() << endl;
    exit(-1);
  }

  parseMazeString();
}

Maze::~Maze() {
  delete tileShader;
}

float Maze::getTileWidth() {
  return tileWidth;
}

float Maze::getTileDepth() {
  return tileDepth;
}

float Maze::getTileSpacing() {
  return tileSpacing;
}

MoveSequence Maze::getMove(unsigned int index) {
  MoveSequence move;
  if (index >= mazeString.length() - 1) {
    move.distances.push_back(0.0f);
    move.directions.push_back(aiVector3D(0, 0, 0));
    return move;
  }

  char prevTile;
  if (index > 0) {
    prevTile = mazeString[index - 1];
  } else {
    prevTile = mazeString[index];
  }

  char tile = mazeString[index];

  cout << "tile " << tile <<  " prev " << prevTile
      << endl;
  //Starting a descent
  if ((prevTile == 'f' || prevTile == 'l' || prevTile == 'r') && tile == 'd') {
    //We have to go over the hump to start heading town a streak of downard tiles
    float distanceToTile = .11;
    float distanceAcrossTile = tileDepth + .05;
    move.distances.push_back(distanceAcrossTile);
    move.distances.push_back(distanceToTile);
    move.directions.push_back(aiVector3D(0, -1, 0));
    move.directions.push_back(aiVector3D(0, 0, 1));
  } else if (tile == 'f') {
    //If we are going forward we have to change how we move
    //given if we came from up or down since we have to cross
    //the height of a brick or less
    if (prevTile == 'd') {
      move.distances.push_back(tileDepth + tileSpacing - .015);
    } else if (prevTile == 'u') {
      move.distances.push_back(tileDepth + tileSpacing + .1);
    } else {
      move.distances.push_back(tileDepth + tileSpacing);
    }
    move.directions.push_back(aiVector3D(0, 0, 1));
  } else if (tile == 'l') {
    move.distances.push_back(tileDepth + tileSpacing);
    move.directions.push_back(aiVector3D(-1, 0, 0));
  } else if (tile == 'r') {
    move.distances.push_back(tileDepth + tileSpacing);
    move.directions.push_back(aiVector3D(1, 0, 0));
  } else if (tile == 'd') {
    move.distances.push_back(tileDepth + tileSpacing);
    move.directions.push_back(aiVector3D(0, -1, 0));
  } else if (tile == 'u') {
    move.distances.push_back(tileDepth + .05);
    move.directions.push_back(aiVector3D(0, 1, 0));
  }
  return move;
}

int Maze::getNumTiles() {
  return mazeString.length();
}

void Maze::selectedInc() {
  if (selectedTile == mazeString.length() - 1) return;
  selectedTile++;
}

void Maze::selectedDec() {
  if (selectedTile == 0) return;
  selectedTile--;
}

void Maze::mazeStringIs(string mazeString_) {
  mazeString = mazeString_;
  parseMazeString();
}

void Maze::parseMazeString() {
  for (unsigned int i = numTiles; i < mazeString.length(); i++) {
    tileData.push_back(TileData());
  }
  numTiles = mazeString.length();
}

void Maze::addTurret(TurretFactory::TurretType type) {
  TileData data = tileData.at(selectedTile);
  data.addTurret(type);
  tileData.erase(tileData.begin() + selectedTile);
  tileData.insert(tileData.begin() + selectedTile, data);
}

void Maze::render(float framerate) {
  GL_CHECK(glUseProgram(tileShader->programID()));

  numTilesLeft = numTilesRight = numTilesDown = numTilesUp = numTilesForward
      = numUpDownRuns = 0;

  glPushMatrix();
  //render first tile
  tile.render(tileShader->programID(), false);

  for (unsigned int i = 0; i < mazeString.length(); i++) {
    addTile(mazeString[i], i == selectedTile, i);
  }

  turretFactory.updateTime(3.0f * framerate);

  glPopMatrix();
  GL_CHECK(glUseProgram(0));
}

void Maze::addTile(char tile, bool selected, unsigned int index) {
  switch (tile) {
    case 'f':
      renderTileForward(selected, index);
      break;
    case 'l':
      renderTileLeft(selected, index);
      break;
    case 'r':
      renderTileRight(selected, index);
      break;
    case 'u':
      renderTileUp(selected, index);
      break;
    case 'd':
      renderTileDown(selected, index);
      break;
  }
}

void Maze::renderTileForward(bool selected, unsigned int index) {
  numTilesForward++;
  //Add spacing, move forward, and render
  glTranslatef(0, 0, tileSpacing);
  glTranslatef(0, 0, tileDepth);
  tile.render(tileShader->programID(), selected);
  renderTurrets(index);
}

void Maze::renderTileLeft(bool selected, unsigned int index) {
  numTilesLeft++;
  //Add spacing, move left, and render
  glTranslatef(-tileSpacing, 0, 0);
  glTranslatef(-tileWidth, 0, 0);
  tile.render(tileShader->programID(), selected);
  renderTurrets(index);
}

void Maze::renderTileRight(bool selected, unsigned int index) {
  numTilesRight++;
  //Add spacing move right and render
  glTranslatef(tileSpacing, 0, 0);
  glTranslatef(tileWidth, 0, 0);
  tile.render(tileShader->programID(), selected);
  renderTurrets(index);
}

void Maze::renderTileUp(bool selected, unsigned int index) {
  numTilesUp++;

  //Reset original matrix and put it back
  //on the stack
  glPopMatrix();
  glPushMatrix();

  float displacementLR = numTilesRight - numTilesLeft;
  //don't want to account for this feller entirely yet
  float displacementUD = numTilesUp - numTilesDown - .5;
  //we add half to get it to move to the end of the last tile
  float displacementForward = numTilesForward + .5;

  glTranslatef(
      displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1
          * (numUpDownRuns));

  glRotatef(-90, 1, 0, 0);
  tile.render(tileShader->programID(), selected);
  renderTurrets(index);
  //reset original matrix
  glPopMatrix();
  glPushMatrix();

  //want to move the next tile entirely past us
  //See if this tile ends a run of up tiles
  if (index < mazeString.length() - 1 && mazeString[index + 1] != 'u') {
    numUpDownRuns++;
  }
  //Reset the number of displacementUD to be the number we have seen in the
  //two directions
  displacementUD = numTilesUp - numTilesDown - .05;
  //Want to move forward only equal to the height of this block
  //which is fixed at .1; this is the .1;
  displacementForward = numTilesForward;
  //Move us to this tile
  glTranslatef(
      displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1
          * (numUpDownRuns));
}

void Maze::renderTileDown(bool selected, unsigned int index) {
  numTilesDown++;
  //Reset original matrix and put it back
  //on the stack
  glPopMatrix();
  glPushMatrix();

  float displacementLR = numTilesRight - numTilesLeft;
  //don't want to account for this feller entirely yet, but we also need to
  //account for the fact that this 90 degree roation places it at an
  //overlay of a tile height
  float displacementUD = numTilesUp - numTilesDown + .5 - .1 - tileSpacing;
  //we add half to get it to move to the end of the last tile; have to move
  //it a height extra since it rotates in as opposed to away
  float displacementForward = numTilesForward + .5 + .1 + tileSpacing;

  glTranslatef(
      displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1
          * (numUpDownRuns));

  glRotatef(90, 1, 0, 0);
  tile.render(tileShader->programID(), selected);
  renderTurrets(index);
  //reset original matrix
  glPopMatrix();
  glPushMatrix();

  //want to move the next tile entirely past us
  //See if this tile ends a run of up tiles
  if (index < mazeString.length() - 1 && mazeString[index + 1] != 'd') {
    numUpDownRuns++;
  }

  //Reset the number of displacementUD to be the number we have seen in the
  //two directions
  displacementUD = numTilesUp - numTilesDown;
  //Want to move forward only equal to the height of this block
  //which is fixed at .1; this is the .1;
  displacementForward = numTilesForward;
  //Move us to this tile
  glTranslatef(
      displacementLR * tileWidth + displacementLR * tileSpacing,
      displacementUD * tileDepth + displacementUD * tileSpacing,
      displacementForward * tileDepth + displacementForward * tileSpacing + .1
          * (numUpDownRuns));
}

void Maze::renderTurrets(unsigned int index) {
  set<TurretFactory::TurretType> tileTurrets = tileData.at(index).getTurrets();

  for (set<TurretFactory::TurretType>::iterator iter = tileTurrets.begin(); iter
      != tileTurrets.end(); ++iter) {
    turretFactory.getTurret(*iter)->render();
  }

}
