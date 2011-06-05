#include <iostream>
#include <OGRE/Ogre.h>

#include "Level\iceLevelManager.h"
#include "Level\iceLevel.h"
#include "iceGame.h"

template<> iceLevelManager* Ogre::Singleton<iceLevelManager>::ms_Singleton = 0;

iceLevelManager::iceLevelManager(): _numLevels(0){
	_log = iceGame::getGameLog();
	_levels.push_back(new iceLevel(1,"level1", "phase1Trajectory", "phase1Enemies"));
	_levels.push_back(new iceLevel(2,"level1", "phase1Trajectory", "phase1Enemies"));
	_numLevels = 2;
	_dotSceneLoader = new DotSceneLoader();
}

iceLevelManager::~iceLevelManager() {

    // destroy levels
    std::vector<iceLevel*>::iterator i;
    for (i = _levels.begin(); i != _levels.end(); ++i)
        delete (*i);
	delete _dotSceneLoader;
	_log->logMessage("iceLevelManager::~iceLevelManager()");
}

iceLevelManager& iceLevelManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

iceLevelManager* iceLevelManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

iceLevel* iceLevelManager::getIceLevel(const int id) {
    // find level by id
    std::vector<iceLevel*>::iterator it;
    for (it = _levels.begin(); it != _levels.end(); ++it)
        if ((*it)->getId() == id)
            return (*it);

    return 0;
}

int iceLevelManager::getNumLevels() const {
    return _numLevels;
}

DotSceneLoader* iceLevelManager::getDotSceneLoader()
{
	return _dotSceneLoader;
}
