#include "States\iceState.h"
#include "iceGame.h"

iceState::iceState(
				   iceSoundManager* soundManager,
				   iceLevelManager* levelManager
				   )
   :
	_loaded(false),
	_soundManager(soundManager),
	_levelManager(levelManager)
{
    _log = iceGame::getGameLog();
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
