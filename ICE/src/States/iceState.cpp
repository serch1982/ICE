#include "States\iceState.h"

iceState::iceState(iceStateManager* stateManager): _stateManager(stateManager), _loaded(false) {
    _log = Ogre::LogManager::getSingleton().getLog("iceLog.log");
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
