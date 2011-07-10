#include <iostream>
#include <cmath>

#include <OGRE/Ogre.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "States\iceStateMenu.h"
#include "States\iceStateManager.h"
#include <Sound\iceSoundManager.h>
#include "iceGame.h"


iceStateMenu::iceStateMenu(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	: iceState(soundManager,levelManager, hikariManager)
{
    _log->logMessage("iceStateMenu::iceStateMenu()");
	_nextICEStateId = MAINMENU;
}

iceStateMenu::~iceStateMenu() {
    _log->logMessage("iceStateMenu::~iceStateMenu()");
    clear();
}

void iceStateMenu::load() {
    if (!_loaded) {
        _log->logMessage("iceStateMenu::load()");
        _loaded = true;

        //light
        _sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

		//show cursor
		iceSdkTray::getInstance()->showCursor();

		try{
			_hikariMenu = _hikariManager->createFlashOverlay("menu", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
			_hikariMenu->load("menu.swf"); //"menu.swf"
			_hikariMenu->setTransparent(false, true);
			_hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &iceStateMenu::menuExitClick));
			_hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &iceStateMenu::menuPlayClick));
			_hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &iceStateMenu::menuContinueClick));
		}catch(char* ex) {
			_log->logMessage(ex);
		}

		_soundManager->loadResourcesMenu();
		_soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);
    }

}

Hikari::FlashValue iceStateMenu::menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = EXIT;
	return FLASH_VOID;
}

Hikari::FlashValue iceStateMenu::menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = PLAY;
	return FLASH_VOID;
}

Hikari::FlashValue iceStateMenu::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = PLAY;
	return FLASH_VOID;
}

void iceStateMenu::clear() {
    if (_loaded) {
        _log->logMessage("iceStateMenu::clean()");
        _loaded = false;
		_hikariManager->destroyFlashControl("menu");
		_soundManager->StopAllSounds();
		_soundManager->unloadResourcesMenu();
    }
}


void iceStateMenu::update(Ogre::Real evt) {
	_hikariManager->update();
	//prueba
	_soundManager->update( Ogre::Vector3::ZERO, Ogre::Quaternion::ZERO, evt);
	
}


bool iceStateMenu::keyPressed(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStateMenu::keyReleased(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStateMenu::mouseMoved(const OIS::MouseEvent &arg) {
	iceSdkTray::getInstance()->moveMouse(arg);
	_hikariManager->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || _hikariManager->injectMouseWheel(arg.state.Z.rel);
    return true;
}

bool iceStateMenu::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_hikariManager->injectMouseDown(id);
    return true;
}

bool iceStateMenu::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_hikariManager->injectMouseUp(id);
    return true;
}


ICEStateId iceStateMenu::getStateId()
{
	return MAINMENU;
}

