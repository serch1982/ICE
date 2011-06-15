#include "States\iceState.h"
#include "iceGame.h"

iceState::iceState(iceStateManager* stateManager,
				   iceSoundManager* soundManager)
   :_stateManager(stateManager),
	_loaded(false),
	mSoundManager(soundManager)
{
    _log = iceGame::getGameLog();
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
