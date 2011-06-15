#include <iostream>

#include <OGRE/Ogre.h>
#include "States\iceStatePlay.h"
#include "States\iceStateManager.h"
#include "Level\iceLevelManager.h"
#include "iceGame.h"
#include "Logic\iceLogicLua.h"

iceStatePlay::iceStatePlay(iceStateManager* stateManager,
						   iceSoundManager* soundManager)
	:iceState(stateManager, soundManager),
	mSoundManager(soundManager),
	_levelID(1) {
    _log->logMessage("iceStatePlay::iceStatePlay()");
	_nextICEStateId = Play;
}

iceStatePlay::~iceStatePlay() {
    _log->logMessage("iceStatePlay::~iceStatePlay()");
    clear();
}

void iceStatePlay::load() {
    if (!_loaded) {
        _log->logMessage("iceStatePlay::load()");
        _loaded = true;

        // light
        _sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
		
		//new player instance
        _player = new icePlayer();

        // load level
        _level = iceLevelManager::getSingleton().getIceLevel(_levelID);
        _level->load(*_player, _mEnemies);

		//hide cursor
		iceSdkTray::getInstance()->hideCursor();

		//load phisics
		mPhisics.initialize(_player, &_mEnemies,_player->getAllBullets(),_player->getAllBullets());

		//load lua logic
		std::string path = _stateManager->getPathRoot() + "\\logiclua.lua";
		iceLogicLua::getInstance()->RunFile(path.c_str()); 

		//load HUD
		try{
			_hikariHUD = _stateManager->getHikariMgr()->createFlashOverlay("HUD",iceGame::getCamera()->getViewport(), iceGame::getCamera()->getViewport()->getActualWidth(), iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
			_hikariHUD->load("HUD.swf");
			_hikariHUD->setTransparent(true, true);
		}catch(char* ex) {
			_log->logMessage(ex);
		}

    }

}
void iceStatePlay::clear() {
    if (_loaded) {
        _log->logMessage("iceStatePlay::limpiar()");
        _loaded = false;

        delete _player;
        _level->unload();
		//clean scene manager
        _sceneManager->clearScene();

		//Deleting enemies
		for (unsigned int i=0;i<_mEnemies.size();i++)
			delete _mEnemies[i];

		//destroy HUD
		_stateManager->getHikariMgr()->destroyFlashControl("HUD");
    }
}


void iceStatePlay::update(Ogre::Real evt) {
	//player
    _player->update(evt);
	//enemies
	for (_revit_mEnemies = _mEnemies.rbegin(); _revit_mEnemies != _mEnemies.rend(); ++_revit_mEnemies) {
		iceLogicLua::getInstance()->getEnemyLogicState((*_revit_mEnemies),evt);
        (*_revit_mEnemies)->update(evt);
    }
	//HUD
	_stateManager->getHikariMgr()->update();
	setHUDLife(_player->getCurrentLife());
	setHUDWeapon(_player->getCurrentWeaponName());

	//phisics
	mPhisics.processPlayerBullets();

	//fx
	/*if(_player->getIsShooting()){
		switch(_player->getCurrentWeapon())
		{
			case  MACHINEGUN:
			
				break;
			case SHOTGUN:
				break;
			case MISILE_LAUNCHER:
				break;
			default:
				break;
		}*/
	
	//}

	//chivatos of the camera
    iceSdkTray::getInstance()->updateScreenInfo( 0, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().x));
    iceSdkTray::getInstance()->updateScreenInfo( 1, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().y));
    iceSdkTray::getInstance()->updateScreenInfo( 2, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().z));
    iceSdkTray::getInstance()->updateScreenInfo( 3, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().w));
    iceSdkTray::getInstance()->updateScreenInfo( 4, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().x));
    iceSdkTray::getInstance()->updateScreenInfo( 5, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().y));
    iceSdkTray::getInstance()->updateScreenInfo( 6, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().z));
}


bool iceStatePlay::keyPressed(const OIS::KeyEvent &arg) {
	if (arg.key == OIS::KC_P)
    {
        this->_nextICEStateId = Pause;
    }
	
    return true;
}

bool iceStatePlay::keyReleased(const OIS::KeyEvent &arg) {
	
    return true;
}

bool iceStatePlay::mouseMoved(const OIS::MouseEvent &arg) {
	_player->processMouseMoved(arg.state.X.rel,arg.state.Y.rel,arg.state.Z.abs);
    return true;
}

bool iceStatePlay::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	if(id==0) 
		_player->setIsShooting(true);
    return true;
}

bool iceStatePlay::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	if(id==0) _player->setIsShooting(false);
    return true;
}

void iceStatePlay::setLevelToLoad(int levelID){
	_levelID = levelID;
}

ICEStateId iceStatePlay::getStateId()
{
	return Play;
}

void iceStatePlay::setHUDLife(int life){
	_hikariHUD->callFunction("setLife",Hikari::Args(life));
}

void iceStatePlay::setHUDWeapon(char* name){
	_hikariHUD->callFunction("setWeapon", Hikari::Args(name));
}
