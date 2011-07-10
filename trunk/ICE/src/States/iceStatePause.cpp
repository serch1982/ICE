#include <iostream>
#include <cmath>

#include <OGRE/Ogre.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "States\iceStatePause.h"
#include "States\iceStateManager.h"
#include "iceGame.h"


iceStatePause::iceStatePause(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState(soundManager,levelManager, hikariManager)
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

		try{
			_hikariMenu = _hikariManager->createFlashOverlay("pause", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
			_hikariMenu->load("menu.swf"); //"menu.swf"
			_hikariMenu->setTransparent(false, true);
			_hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &iceStatePause::menuExitClick));
			_hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &iceStatePause::menuPlayClick));
			_hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &iceStatePause::menuContinueClick));
			//set the menu to pause mode
			_hikariMenu->callFunction("inGame",Hikari::Args(true));
		}catch(char* ex) {
			_log->logMessage(ex);
		}
    }

}

Hikari::FlashValue iceStatePause::menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = EXIT;
	return FLASH_VOID;
}

Hikari::FlashValue iceStatePause::menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = PLAY;
	_hikariMenu->hide();
	return FLASH_VOID;
}

Hikari::FlashValue iceStatePause::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = PLAY;
	_hikariMenu->hide();
	return FLASH_VOID;
}

void iceStatePause::clear() {
    if (_loaded) {
        _log->logMessage("iceStatePause::clean stateMenu()");
        _loaded = false;
		_hikariManager->destroyFlashControl("pause");
    }
}


void iceStatePause::update(Ogre::Real evt) {
	_hikariManager->update();
}


bool iceStatePause::keyPressed(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStatePause::keyReleased(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStatePause::mouseMoved(const OIS::MouseEvent &arg) {
	iceSdkTray::getInstance()->moveMouse(arg);
	_hikariManager->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || _hikariManager->injectMouseWheel(arg.state.Z.rel);
    return true;
}

bool iceStatePause::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_hikariManager->injectMouseDown(id);
    return true;
}

bool iceStatePause::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_hikariManager->injectMouseUp(id);
    return true;
}


ICEStateId iceStatePause::getStateId()
{
	return PAUSE;
}

