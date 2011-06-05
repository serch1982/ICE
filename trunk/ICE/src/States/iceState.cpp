#include "States\iceState.h"
#include "iceGame.h"

iceState::iceState(iceStateManager* stateManager): _stateManager(stateManager), _loaded(false) {
    _log = iceGame::getGameLog();
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
