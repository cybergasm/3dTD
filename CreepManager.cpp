/*
 * CreepManager.cpp
 *
 *  Created on: Mar 17, 2012
 *      Author: emint
 */

#include "CreepManager.h"

CreepManager::CreepManager(Maze* maze_, TurretFactory* turretFactory_) :
  nextCreepTime(0), maze(maze_), turretFactory(turretFactory_) {
  // TODO Auto-generated constructor stub
  creepShader = new Shader("shaders/creep");
  if (!creepShader->loaded()) {
    cerr << "Error loading creep shader." << endl;
    cerr << creepShader->errors() << endl;
    exit(-1);
  }

  creepTexture.LoadFromFile("models/creep_opacity_texture1.jpg");
}

CreepManager::~CreepManager() {
  // TODO Auto-generated destructor stub
}

void CreepManager::updateTime(float framerate) {
  if (nextCreepTime <= 0.0f) return;

  nextCreepTime -= framerate * 10.0f;
}

void CreepManager::updateCreeps() {
  if (nextCreepTime <= 0.0f) {
    creeps .push_back(Creep(creepShader, maze, &creepTexture, turretFactory));
    nextCreepTime = (10.0f + rand() % 20);
  }

  /**
   * Figure out which creeps are dead
   */
  vector<int> deadCreeps;
  int creep = 0;
  for (vector<Creep>::iterator iter = creeps.begin(); iter != creeps.end(); ++iter) {
    if ((*iter).getStatus() == Creep::CREEP_DEAD || (*iter).getStatus()
        == Creep::CREEP_ESCAPED) {
      deadCreeps.push_back(creep);
    }
    creep++;
  }
  //actually delete the creeps
  for (vector<int>::iterator iter = deadCreeps.begin(); iter
      != deadCreeps.end(); ++iter) {
    creeps.erase(creeps.begin() + (*iter));
  }
}

void CreepManager::renderCreeps(float framerate) {
  for (vector<Creep>::iterator iter = creeps.begin(); iter != creeps.end(); ++iter) {
    (*iter).update(framerate);
    (*iter).render(framerate);
  }
}
