 #include <iostream>

#include <OGRE/Ogre.h>
#include "States\iceStatePlay.h"
#include "States\iceStateManager.h"
#include "Level\iceLevelManager.h"
#include "iceGame.h"
#include "Logic\iceLogicLua.h"
#include "Utils\iceLoadingBar.h"

#define DOUBLE_KEY_TIME 0.5

iceStatePlay::iceStatePlay(
	iceSoundManager* soundManager
	)
	:iceState(soundManager),
	_levelID(1) {
    _log->logMessage("iceStatePlay::iceStatePlay()");
	_nextICEStateId = PLAY;
	visibleBoundingBoxes = false;
	
	mCurrentTime = 0;
	mCurrentCutScene = NULL;

	mUpCounter = 0;
	mDownCounter = 0;
	mLeftCounter = 0;
	mRightCounter = 0;
}

iceStatePlay::~iceStatePlay() {
    _log->logMessage("iceStatePlay::~iceStatePlay()");
	 //delete _player;
	_player.reset();
    clear();
}

void iceStatePlay::load() {

	mFightingAgainstBoss = false;

	if(!_loaded){
			_log->logMessage("iceStatePlay::load()");
			_loaded = true;

			//level manager
			_levelManager = new iceLevelManager();	

			//particle system
			mIceParticleMgr = iceParticleMgrPtr(new iceParticleMgr());
			mIceParticleMgr->initialize();

			//bullet manager
			mIceBulletMgr = iceBulletMgrPtr(new iceBulletMgr());
			mIceBulletMgr->initialize();

			////new player instance
			if(!icePlayer::getSingletonPtr()) {
				_player = icePlayerPtr(new icePlayer());
			}else{
				_player->initPlayer();
			}
			_player->getNode()->setVisible(false);

			//hide cursor
			iceSdkTray::getInstance()->hideCursor();

			//load lua logic
			iceLogicLua::getInstance()->runAllFiles();

			// load level
			_level = iceLevelManager::getSingleton().getIceLevel(_levelID);
			_level->load(_mEnemies, _mCutScenes, _soundManager);
			if( _levelID == 2 ){
				_mMagmaton = (iceBoss*) _mEnemies.back();
				_mEnemies.pop_back();
			}else{
				_mMagmaton = NULL;
			}

			//load physics
			mPhysics = icePhysicsPtr(new icePhysics());
			mPhysics->initialize(_level->getTerrain(), &_mEnemies, _level->getStaticPhisicSceneObjects());

			//show HUD
			mHUD = iceGame::getUI()->getHUD();
			mHUD->show();

			mPropAnimations = _level->getPropAnimations();

			_damageTextManager = new iceDamageTextManager();

			//post process
			_postProcessManager = new icePostProcessManager();
			icePostProcessManager::getSingleton().enableSoftBlur();
			icePostProcessManager::getSingleton().enableToon();
			icePostProcessManager::getSingleton().enableDepthOfField();

			mCurrentTime = 0;
			_player->getNode()->setVisible(true);
			mState = NORMAL;
	}
}

void iceStatePlay::clear() {
    if (_loaded) {
        _log->logMessage("iceStatePlay::limpiar()");
        _loaded = false;

		//delete level;
		_level->unload( _soundManager );
		delete _levelManager;

		//Deleting enemies
		for (unsigned int i=0;i<_mEnemies.size();i++)
			delete _mEnemies[i];
		if(_mMagmaton)
			delete _mMagmaton;

		//delete bullet manager;
		mIceBulletMgr.reset();
		//delete particle manager;
		mIceParticleMgr.reset();
		//delete Physics;
		mPhysics.reset();

		//delete postprocess
		icePostProcessManager::getSingleton().disableBlur();
		icePostProcessManager::getSingleton().disableToon();
		icePostProcessManager::getSingleton().disableDepthOfField();
		delete _postProcessManager;	

		//delete text Manager
		delete _damageTextManager;

		//clean scene manager
        _sceneManager->clearScene();

		//hide HUD
		mHUD->hide();
		mPropAnimations.clear();

    }
}


void iceStatePlay::update(Ogre::Real evt)
{
	switch(mState)
	{
		case NORMAL:
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
				//props
				for(unsigned int i=0;i<mPropAnimations.size();i++)
				{
					mPropAnimations[i]->addTime(evt);
				}

				//bullets
				mIceBulletMgr->update(evt, visibleBoundingBoxes);

				//player
				_player->update(evt);
				_player->setDebug(visibleBoundingBoxes);

				//update game Physics 
				mPhysics->update();

				//enemies
				for (_revit_mEnemies = _mEnemies.rbegin(); _revit_mEnemies != _mEnemies.rend(); ++_revit_mEnemies) {
					iceEnemy* enemy = *_revit_mEnemies;
					iceLogicLua::getInstance()->getEnemyLogicState(enemy,evt);
					enemy->setDebug(visibleBoundingBoxes);
					enemy->update(evt);
				}

				if(_mMagmaton)
				{
					iceLogicLua::getInstance()->getEnemyLogicState(_mMagmaton,evt);
					_mMagmaton->setDebug(visibleBoundingBoxes);
					_mMagmaton->update(evt);
				}
				//HUD

				//setHUDWeapon(_player->getCurrentWeaponName());

				//ShowDamage
				iceDamageTextManager::getSingleton().update(evt);

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
				iceSdkTray::getInstance()->updateScreenInfo( 6, Ogre::StringConverter::toString(iceGame::getCamera()->getDerivedOrientation().y));

				mCurrentTime += evt*_player->getTimeMultiplier();

				if(!_player->isAlive())
				{
					_player->die();
					mState = PLAYER_DYING;
				}
			}

			//update particles
			iceParticleMgr::getSingletonPtr()->update(evt);
			break;
		case CHEATING:
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
				//props
				for(unsigned int i=0;i<mPropAnimations.size();i++)
				{
					mPropAnimations[i]->addTime(evt);
				}

				//bullets
				mIceBulletMgr->update(evt, visibleBoundingBoxes);

				//player
				_player->update(evt);
				_player->setDebug(visibleBoundingBoxes);

				//update game Physics 
				mPhysics->update();

				//enemies
				for (_revit_mEnemies = _mEnemies.rbegin(); _revit_mEnemies != _mEnemies.rend(); ++_revit_mEnemies) {
					iceEnemy* enemy = *_revit_mEnemies;
					iceLogicLua::getInstance()->getEnemyLogicState(enemy,evt);
					enemy->setDebug(visibleBoundingBoxes);
					enemy->update(evt);
				}

				if(_mMagmaton)
				{
					iceLogicLua::getInstance()->getEnemyLogicState(_mMagmaton,evt);
					_mMagmaton->setDebug(visibleBoundingBoxes);
					_mMagmaton->update(evt);
				}
				//HUD

				//setHUDWeapon(_player->getCurrentWeaponName());

				//ShowDamage
				iceDamageTextManager::getSingleton().update(evt);

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

				mCurrentTime += evt*_player->getTimeMultiplier();
			}

			//update particles
			iceParticleMgr::getSingletonPtr()->update(evt);
			break;
		case PLAYER_DYING:
			//bullets
			mIceBulletMgr->update(evt, visibleBoundingBoxes);

			//player
			_player->update(evt);
			_player->setDebug(visibleBoundingBoxes);

			//update particles
			iceParticleMgr::getSingletonPtr()->update(evt);

			mCurrentTime += evt*_player->getTimeMultiplier();

			if(!_player->isDying()) //not dying => is dead
			{
				mState = SHOWIN_GAMEOVER;
				mHUD->showGameover();
			}

			break;
		case SHOWIN_GAMEOVER:
			break;
	}
}


bool iceStatePlay::keyPressed(const OIS::KeyEvent &arg) {
	if (arg.key == OIS::KC_ESCAPE)
    {
		if(mCurrentCutScene)
			mCurrentCutScene->stop();
		else
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
		mUpCounter = DOUBLE_KEY_TIME;
		_player->setMovingUp(true);
    }
	else if(arg.key == OIS::KC_S)   // down
    {
		if(mDownCounter > 0)
		{
			_player->brake();
		}
		mDownCounter = DOUBLE_KEY_TIME;
		_player->setMovingDown(true);
    }	
	else if(arg.key == OIS::KC_A)   // left
    {
		if(mLeftCounter > 0)
		{
			_player->barrelLeft();
		}
		mLeftCounter = DOUBLE_KEY_TIME;
		_player->setMovingLeft(true);
    }	
	else if(arg.key == OIS::KC_D)   // right
    {
		if(mRightCounter > 0)
		{
			_player->barrelRight();
		}
		mRightCounter = DOUBLE_KEY_TIME;
		_player->setMovingRight(true);
    }
	else if(arg.key == OIS::KC_Z)   // heal
    {
		_player->levelUp();
    }
	else if(arg.key == OIS::KC_U)   // heal
    {
		mHUD->showFrame();
    }
	else if(arg.key == OIS::KC_I)   // heal
    {
		mHUD->hideFrame();
    }
	else if(arg.key == OIS::KC_C)
    {
		if(mState == CHEATING)
		{
			mState = NORMAL;
			mHUD->setCheating(false);
		}
		else
		{
			mState = CHEATING;
			mHUD->setCheating(true);
		}
    }
	else if(arg.key == OIS::KC_SPACE)
    {
		if(mState == SHOWIN_GAMEOVER)
		{
			mHUD->hideGameover();
			iceGame::getStateManager()->goToMainMenu();
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
		_player->heal();
    return true;
}

bool iceStatePlay::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	if(id==0) _player->setIsShooting(false);
    return true;
}

void iceStatePlay::setLevelToLoad(int levelID){
	_levelID = levelID;
}

int iceStatePlay::getLevelLoaded(){
	return _levelID;
}

int iceStatePlay::getNextLevel(){
	if(_levelManager){
		if(_levelID  + 1 > _levelManager->getNumLevels()) {
			_levelID = 1;
			return 999;
		}else{
			return _levelID + 1;
		}
	} else{
		return 1;
	}
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
		_level->setDebugSceneObjects(false);
		visibleBoundingBoxes = false;
	}
	else
	{
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

