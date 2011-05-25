#include <iostream>
#include <cmath>

#include <OGRE/Ogre.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "States\iceStateMenu.h"
#include "States\iceStateManager.h"
#include "iceGame.h"
#include "Sound\iceMusicManager.h"


iceStateMenu::iceStateMenu(iceStateManager* stateManager): iceState(stateManager) {
    _log->logMessage("iceStateMenu::iceStateMenu()");
	_nextICEStateId = MainMenu;
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
			_hikariMenu = _stateManager->getHikariMgr()->createFlashOverlay("menu", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
			_hikariMenu->load("menu.swf"); //"menu.swf"
			_hikariMenu->setTransparent(false, true);
			_hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &iceStateMenu::menuExitClick));
			_hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &iceStateMenu::menuPlayClick));
			_hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &iceStateMenu::menuContinueClick));
		}catch(char* ex) {
			_log->logMessage(ex);
		}

		_music = iceMusicManager::getSingleton().load("menu.mp3");
		_music->play( );
    }

}

Hikari::FlashValue iceStateMenu::menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = Exit;
	return FLASH_VOID;
}

Hikari::FlashValue iceStateMenu::menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = Play;
	return FLASH_VOID;
}

Hikari::FlashValue iceStateMenu::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	this->_nextICEStateId = Play;
	return FLASH_VOID;
}

void iceStateMenu::clear() {
    if (_loaded) {
        _log->logMessage("iceStateMenu::clean()");
        _loaded = false;
		_stateManager->getHikariMgr()->destroyAllControls();
		_music->stop();
    }
}


void iceStateMenu::update(Ogre::Real evt) {
	_stateManager->getHikariMgr()->update();
}


bool iceStateMenu::keyPressed(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStateMenu::keyReleased(const OIS::KeyEvent &arg) {
    return true;
}

bool iceStateMenu::mouseMoved(const OIS::MouseEvent &arg) {
	iceSdkTray::getInstance()->moveMouse(arg);
	_stateManager->getHikariMgr()->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || _stateManager->getHikariMgr()->injectMouseWheel(arg.state.Z.rel);
    return true;
}

bool iceStateMenu::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_stateManager->getHikariMgr()->injectMouseDown(id);
    return true;
}

bool iceStateMenu::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_stateManager->getHikariMgr()->injectMouseUp(id);
    return true;
}


ICEStateId iceStateMenu::getStateId()
{
	return MainMenu;
}

