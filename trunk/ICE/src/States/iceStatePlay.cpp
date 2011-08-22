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
	iceLevelManager* levelManager
	)
	:iceState(soundManager,levelManager),
	_levelID(1) {
    _log->logMessage("iceStatePlay::iceStatePlay()");
	_nextICEStateId = PLAY;
	visibleBoundingBoxes = false;

	mRightClick = false;

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

			//bullet manager
			mIceBulletMgr = iceBulletMgrPtr(new iceBulletMgr());

			//new player instance
			_player = icePlayerPtr(new icePlayer());

			//hide cursor
			iceSdkTray::getInstance()->hideCursor();

			//load sounds
			_soundManager->loadLevel1();
			_soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);

			//load lua logic
			iceLogicLua::getInstance()->runAllFiles();

			// load level
			_level = iceLevelManager::getSingleton().getIceLevel(_levelID);
			_level->load(_mEnemies, _mCutScenes);

			//load phisics
			mPhysics = icePhysicsPtr(new icePhysics());
			mPhysics->initialize(_level->getTerrain(), &_mEnemies, _level->getStaticPhisicSceneObjects());

			//show HUD
			mHUD = iceGame::getUI()->getHUD();
			mHUD->show();

			//post process
			icePostProcessManager::getSingleton().enableSoftBlur();
			icePostProcessManager::getSingleton().enableToon();

		}

    }else if( _levelID == 2 ){
		// Boss LEVEL
		if( !_loaded ){
						_log->logMessage("iceStatePlay::load()");
			_loaded = true;

			// light
			_sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
		
			//particle system
			mIceParticleMgr = iceParticleMgrPtr(new iceParticleMgr());
			mIceParticleMgr->initialize();

			//bullet manager
			mIceBulletMgr = iceBulletMgrPtr(new iceBulletMgr());

			//new player instance
			_player = icePlayerPtr(new icePlayer());

			//hide cursor
			iceSdkTray::getInstance()->hideCursor();

			//load sounds
			_soundManager->loadLevel1();
			_soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);

			//load lua logic
			iceLogicLua::getInstance()->runAllFiles();

			// load level
			_level = iceLevelManager::getSingleton().getIceLevel(_levelID);
			_level->load(_mEnemies, _mCutScenes);

			//load phisics
			mPhysics = icePhysicsPtr(new icePhysics());
			mPhysics->initialize(_level->getTerrain(), &_mEnemies, _level->getStaticPhisicSceneObjects());

			//show HUD
			mHUD = iceGame::getUI()->getHUD();
			mHUD->show();
		}
	}

}

void iceStatePlay::clear() {
    if (_loaded) {
        _log->logMessage("iceStatePlay::limpiar()");
        _loaded = false;

        //delete _player;
		_player.reset();
		//delete _player;
        _level->unload();

		//delete bullet manager;
		mIceBulletMgr.reset();
		//delete particle manager;
		mIceParticleMgr.reset();
		//delete Physics;
		mPhysics.reset();

		_soundManager->unloadLevel1();

		//clean scene manager
        _sceneManager->clearScene();

		//Deleting enemies
		for (unsigned int i=0;i<_mEnemies.size();i++)
			delete _mEnemies[i];

		//hide HUD
		mHUD->hide();

		//post process
		icePostProcessManager::getSingleton().disableBlur();
		icePostProcessManager::getSingleton().disableToon();
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
		//bullets
		mIceBulletMgr->update(evt, visibleBoundingBoxes);

		//player
		_player->update(evt);
		//enemies
		for (_revit_mEnemies = _mEnemies.rbegin(); _revit_mEnemies != _mEnemies.rend(); ++_revit_mEnemies) {
			iceEnemy* enemy = *_revit_mEnemies;
			if(enemy->isActive() && _player->isPositionBackToPlayer((*_revit_mEnemies)->getNode()->_getDerivedPosition()))
			{
				enemy->desactivate();
			}
			else
			{
				iceLogicLua::getInstance()->getEnemyLogicState(enemy,evt);
				enemy->setDebug(visibleBoundingBoxes);
				enemy->update(evt);
			}
		}
		//HUD
		

		//setHUDWeapon(_player->getCurrentWeaponName());

		//update game Physics 
		mPhysics->update();

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
		if(mRightClick ==  true)
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
		if(mRightClick ==  true)
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
		if(mRightClick ==  true)
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
		if(mRightClick ==  true)
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
		_player->setMovingUp(false);
    }
	else if(arg.key == OIS::KC_S)   // down
    {
		_player->setMovingDown(false);
    }	
	else if(arg.key == OIS::KC_A)   // left
    {
		_player->setMovingLeft(false);
    }	
	else if(arg.key == OIS::KC_D)   // right
    {
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

	if(id== 1) 
		mRightClick = true;
    return true;
}

bool iceStatePlay::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	if(id==0) _player->setIsShooting(false);
	if(id== 1)  mRightClick = false;
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
	//mHUD->callFunction("setLife",Hikari::Args(life));
}

void iceStatePlay::setHUDWeapon(char* name){
	//mHUD->callFunction("setWeapon", Hikari::Args(name));
}

void iceStatePlay::switchBoundingBoxesVisibility(void)
{
	if(visibleBoundingBoxes)
	{
		_player->setDebug(false);
		_level->setDebugSceneObjects(false);
		visibleBoundingBoxes = false;
	}
	else
	{
		_player->setDebug(true);
		_level->setDebugSceneObjects(true);
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
