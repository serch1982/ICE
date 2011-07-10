#include "States\iceState.h"
#include "iceGame.h"

iceState::iceState(
				   iceSoundManager* soundManager,
				   iceLevelManager* levelManager,
				   Hikari::HikariManager *hikariManager
				   )
   :
	_loaded(false),
	_soundManager(soundManager),
	_levelManager(levelManager),
	_hikariManager(hikariManager)
{
    _log = iceGame::getGameLog();
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
