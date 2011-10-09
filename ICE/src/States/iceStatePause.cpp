#include <iostream>
#include <cmath>

#include <OGRE/Ogre.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "States\iceStatePause.h"
#include "States\iceStateManager.h"
#include "iceGame.h"
#include "Utils/iceDamageTextManager.h"


iceStatePause::iceStatePause(
	iceSoundManager* soundManager
	)
	:iceState(soundManager)
{
    _log->logMessage("iceStatePause::iceStatePause()");
	_nextICEStateId = PAUSE;
}

iceStatePause::~iceStatePause() {
    _log->logMessage("iceStatePause::~iceStatePause()");
    clear();
}

void iceStatePause::load() {
    if (!_loaded) {
        _log->logMessage("iceStatePause::load()");
        _loaded = true;

        //light
        _sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
		
		//show cursor
		iceSdkTray::getInstance()->showCursor();

		//show menu
		mMenu = iceGame::getUI()->getMenu();
		mMenu->showPauseMenu();
    }

}
void iceStatePause::clear() {
    if (_loaded) {
        _log->logMessage("iceStatePause::clean stateMenu()");
        _loaded = false;
		mMenu->hide();
    }
}


void iceStatePause::update(Ogre::Real evt) {
	iceDamageTextManager::getSingletonPtr()->hideAll();
}


bool iceStatePause::keyPressed(const OIS::KeyEvent &arg) {
	if (arg.key == OIS::KC_ESCAPE)
    {
		this->_nextICEStateId = PLAY;
    }
    return mMenu->keyPressed(arg);
}

bool iceStatePause::keyReleased(const OIS::KeyEvent &arg) {
    return mMenu->keyReleased(arg);
}

bool iceStatePause::mouseMoved(const OIS::MouseEvent &arg) {
	return mMenu->mouseMoved(arg);
}

bool iceStatePause::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	return mMenu->mousePressed(arg,id);
}

bool iceStatePause::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	return mMenu->mouseReleased(arg,id);
}


ICEStateId iceStatePause::getStateId()
{
	return PAUSE;
}

