#include <iostream>

#include <OGRE/Ogre.h>
#include "States\iceStatePlay.h"
#include "States\iceStateManager.h"
#include "Level\iceLevelManager.h"
#include "iceGame.h"
#include "Logic\iceLogicLua.h"

#define DOUBLE_KEY_TIME 0.5

iceStatePlay::iceStatePlay(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState(soundManager,levelManager, hikariManager),
	_levelID(1) {
    _log->logMessage("iceStatePlay::iceStatePlay()");
	_nextICEStateId = PLAY;
	visibleBoundingBoxes = false;

	mUpCounter = 0;
	mDownCounter = 0;
	mLeftCounter = 0;
	mRightCounter = 0;

	mCurrentTime = 0;
	mCurrentCutScene = NULL;
}

iceStatePlay::~iceStatePlay() {
    _log->logMessage("iceStatePlay::~iceStatePlay()");
    clear();
}

void iceStatePlay::load() {
    if ( _levelID == 1) {
		if( !_loaded ){
			_log->logMessage("iceStatePlay::load()");
			_loaded = true;

			// light
			_sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
		
			//particle system
			mIceParticleMgr = iceParticleMgrPtr(new iceParticleMgr());
			mIceParticleMgr->initialize();

			//new player instance
			_player = new icePlayer();

			// load level
			_level = iceLevelManager::getSingleton().getIceLevel(_levelID);
			_level->load(*_player, _mEnemies, _mCutScenes);

			//hide cursor
			iceSdkTray::getInstance()->hideCursor();

			//load phisics
			mPhysics.initialize(_level->getTerrain(), _player, &_mEnemies, _level->getSceneObjects());

			//load sounds
			_soundManager->loadLevel1();
			_soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);

			//load lua logic
			//Maybe we should choose another way
			std::string path = "./media/scripts/lua/kamikaze.lua";
			iceLogicLua::getInstance()->RunFile(path.c_str());

			//load HUD
			try{
				_hikariHUD = _hikariManager->createFlashOverlay("HUD",iceGame::getCamera()->getViewport(), iceGame::getCamera()->getViewport()->getActualWidth(), iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
				_hikariHUD->load("HUD.swf");
				_hikariHUD->setTransparent(true, true);
			}catch(char* ex) {
				_log->logMessage(ex);
			}

		}

    }else if( _levelID == 2 ){
		// Boss LEVEL
		if( !_loaded ){
			
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

		_soundManager->unloadLevel1();

		//Deleting enemies
		for (unsigned int i=0;i<_mEnemies.size();i++)
			delete _mEnemies[i];

		//destroy HUD
		_hikariManager->destroyFlashControl("HUD");
    }
}


void iceStatePlay::update(Ogre::Real evt)
{
	checkActivableCutScene();

	if(mCurrentCutScene)
	{
		mCurrentCutScene->update(evt);
		if(mCurrentCutScene->hasEnded())
		{
			mCurrentCutScene->rollback();
			mCurrentCutScene = NULL;
		}
	}
	else
	{
		//player
		_player->update(evt);
		//enemies
		for (_revit_mEnemies = _mEnemies.rbegin(); _revit_mEnemies != _mEnemies.rend(); ++_revit_mEnemies) {
			iceLogicLua::getInstance()->getEnemyLogicState((*_revit_mEnemies),evt);
			(*_revit_mEnemies)->update(evt);
		}
		//HUD
		_hikariManager->update();
		//setHUDLife(_player->getCurrentLife());
		//setHUDWeapon(_player->getCurrentWeaponName());

		//phisics
		mPhysics.update();
		//particle system
		mIceParticleMgr->update(evt);

		//ShowDamage
		iceDamageTextManager::getSingleton().update(evt);

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

		if(mUpCounter > 0)
			mUpCounter -= evt;
		if(mDownCounter > 0)
			mDownCounter -= evt;
		if(mLeftCounter > 0)
			mLeftCounter -= evt;
		if(mRightCounter > 0)
			mRightCounter -= evt;

		//chivatos of the camera
		iceSdkTray::getInstance()->updateScreenInfo( 0, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().x));
		iceSdkTray::getInstance()->updateScreenInfo( 1, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().y));
		iceSdkTray::getInstance()->updateScreenInfo( 2, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedPosition().z));
		iceSdkTray::getInstance()->updateScreenInfo( 3, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().w));
		iceSdkTray::getInstance()->updateScreenInfo( 4, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().x));
		iceSdkTray::getInstance()->updateScreenInfo( 5, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().y));
		iceSdkTray::getInstance()->updateScreenInfo( 6, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().z));

		mCurrentTime += evt;
	}
}


bool iceStatePlay::keyPressed(const OIS::KeyEvent &arg) {
	if (arg.key == OIS::KC_P)
    {
        this->_nextICEStateId = PAUSE;
    }	
	else if(arg.key == OIS::KC_F7)   // show bounding boxes
    {
		switchBoundingBoxesVisibility();
    }
	else if(arg.key == OIS::KC_W)   // up
    {
		if(mUpCounter > 0)
		{
			_player->sprint();
		}
		else
		{
			_player->setMovingUp(true);
		}
    }
	else if(arg.key == OIS::KC_S)   // down
    {
		if(mDownCounter > 0)
		{
			_player->brake();
		}
		else
		{
			_player->setMovingDown(true);
		}
    }	
	else if(arg.key == OIS::KC_A)   // left
    {
		if(mLeftCounter > 0)
		{
			_player->barrelLeft();
		}
		else
		{
			_player->setMovingLeft(true);
		}
    }	
	else if(arg.key == OIS::KC_D)   // right
    {
		if(mRightCounter > 0)
		{
			_player->barrelRight();
		}
		else
		{
			_player->setMovingRight(true);
		}
    }
	
    return true;
}

bool iceStatePlay::keyReleased(const OIS::KeyEvent &arg)
{
	if(arg.key == OIS::KC_W)   // up
    {
		mUpCounter = DOUBLE_KEY_TIME;
		_player->setMovingUp(false);
    }
	else if(arg.key == OIS::KC_S)   // down
    {
		mDownCounter = DOUBLE_KEY_TIME;
		_player->setMovingDown(false);
    }	
	else if(arg.key == OIS::KC_A)   // left
    {
		mLeftCounter = DOUBLE_KEY_TIME;
		_player->setMovingLeft(false);
    }	
	else if(arg.key == OIS::KC_D)   // right
    {
		mRightCounter = DOUBLE_KEY_TIME;
		_player->setMovingRight(false);
    }
	
    return true;
}

bool iceStatePlay::mouseMoved(const OIS::MouseEvent &arg) {
	if( _sceneManager->getCurrentViewport()->getCamera()->getName() != "GodCam" )
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
	return PLAY;
}

void iceStatePlay::setHUDLife(int life){
	_hikariHUD->callFunction("setLife",Hikari::Args(life));
}

void iceStatePlay::setHUDWeapon(char* name){
	_hikariHUD->callFunction("setWeapon", Hikari::Args(name));
}

void iceStatePlay::switchBoundingBoxesVisibility(void)
{
	if(visibleBoundingBoxes)
	{
		_player->hideBoundingBox();
		for(unsigned int i=0;i<_mEnemies.size();i++)
		{
			_mEnemies[i]->hideBoundingBox();
		}
		visibleBoundingBoxes = false;
	}
	else
	{
		_player->showBoundingBox();
		for(unsigned int i=0;i<_mEnemies.size();i++)
		{
			_mEnemies[i]->showBoundingBox();
		}
		visibleBoundingBoxes = true;
	}
}

void iceStatePlay::checkActivableCutScene(void)
{
	if(mCurrentCutScene)
		return;

	for(unsigned int i=0;i<_mCutScenes.size();i++)
	{
		Ogre::Real activationTime = _mCutScenes[i]->getActivationTime();
		if(activationTime>=0)
		{
			if(activationTime<mCurrentTime)
			{
				mCurrentCutScene = _mCutScenes[i];
				mCurrentCutScene->play();
				return;
			}
		}
	}
}
