#include "States\iceState.h"
#include "iceGame.h"

iceState::iceState(iceSoundManager* soundManager ):
	_loaded(false),
	_soundManager(soundManager)
{
    _log = iceGame::getGameLog();
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
