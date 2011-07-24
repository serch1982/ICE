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
	iceLevelManager* levelManager
	)
	: iceState(soundManager,levelManager)
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

		mMenu = iceGame::getUI()->getMenu();
		mMenu->showMainMenu();

		_soundManager->loadResourcesMenu();
		_soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);
    }

}

void iceStateMenu::clear() {
    if (_loaded) {
        _log->logMessage("iceStateMenu::clean()");
        _loaded = false;
		mMenu->hide();
		_soundManager->StopAllSounds();
		_soundManager->unloadResourcesMenu();
    }
}


void iceStateMenu::update(Ogre::Real evt) {
	//prueba
	_soundManager->update( Ogre::Vector3::ZERO, Ogre::Quaternion::ZERO, evt);
	
}


bool iceStateMenu::keyPressed(const OIS::KeyEvent &arg) {
	return mMenu->keyPressed(arg);
}

bool iceStateMenu::keyReleased(const OIS::KeyEvent &arg) {
    return mMenu->keyReleased(arg);
}

bool iceStateMenu::mouseMoved(const OIS::MouseEvent &arg) {
	return mMenu->mouseMoved(arg);
}

bool iceStateMenu::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	return mMenu->mousePressed(arg,id);
}

bool iceStateMenu::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	return mMenu->mouseReleased(arg,id);
}


ICEStateId iceStateMenu::getStateId()
{
	return MAINMENU;
}

