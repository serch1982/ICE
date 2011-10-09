#ifndef ICESTATEPLAY_H_
#define ICESTATEPLAY_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "States/iceState.h"
#include "Level\iceLevel.h"
#include "Level\iceCutScene.h"
#include "Utils\iceSdkTray.h"
#include "Entities\icePlayer.h"
#include "Enemies\iceEnemy.h"
#include "Enemies\iceBoss.h"
#include "Particle\iceParticleMgr.h"
#include "Physics\icePhysics.h"
#include "Entities\iceBulletMgr.h"
#include "Sound\iceSoundManager.h"
#include "UI\iceHUD.h"
#include "Utils\iceDamageTextManager.h"
#include "PostProcess\icePostProcessManager.h"
#include "Level\iceLevelManager.h"

class iceLevel;


class iceStatePlay: public iceState{
    public:
        /**
         *  Constructor play state
         */
        iceStatePlay(
			iceSoundManager* soundManager
			);

        /**
         * Destructor
         */
        ~iceStatePlay();

		enum PLAYSTATE
		{
			NORMAL = 0,
			CHEATING = 1,
			PLAYER_DYING = 2,
			SHOWIN_GAMEOVER = 3
		};
        
        /**
         *  load elements
         */
        void load();

        /**
         *  clear elements
         */
        void clear();

        /**
         *  @param evt last time since update.
         *  @param active set the state
         *
         *  update state 
         */
        void update(Ogre::Real evt);


        /**
         *  keypressed event manager
         */
        bool keyPressed(const OIS::KeyEvent &arg);
        
        /**
         *  keyReleased event manager
         */
        bool keyReleased(const OIS::KeyEvent &arg);
        
        /**
         *  mouseMoved event manager
         */
        bool mouseMoved(const OIS::MouseEvent &arg);
        
        /**
         *  mousePressed event manager
         */
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        
        /**
         *  mouseReleased event manager
         */
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

		 /**
         *  get the current icestateid
         */
		ICEStateId getStateId();

		/**
         *  set the level to load
         */
		void setLevelToLoad(int levelID);

		/**
         *  get the current level ID
         */
		int getLevelLoaded();

		/**
         *  get the levels 
         */
		int getNextLevel();

		/**
         *  set the visibility for the boundingboxes
         */
		void switchBoundingBoxesVisibility(void);
    private:
		PLAYSTATE mState;

		//HUD
		iceHUD* mHUD;

		//player
        icePlayerPtr _player;
		//Enemies
		std::vector<iceEnemy*> _mEnemies;
		iceBoss* _mMagmaton;
		bool mFightingAgainstBoss;
		 // inverse iterator
        std::vector<iceEnemy*>::const_reverse_iterator _revit_mEnemies;

		std::vector<iceCutScene*> _mCutScenes;

        iceLevel* _level;
		int _levelID;
		
		//methods to HUD
		/**
         *  set the life of the player, it must be between 0 - 100
         */
		void setHUDLife(int life);
		/**
         *  set the name of the current weapon of the player
         */
		void setHUDWeapon(char* name);

		/**
         *  check the CutScenes
         */
		void checkActivableCutScene(void);

		//phisics
		icePhysicsPtr mPhysics;

		bool visibleBoundingBoxes;

		Ogre::Real mCurrentTime;
		iceCutScene* mCurrentCutScene;

		//particle Manager
		iceParticleMgrPtr mIceParticleMgr;

		//bullet  manager
		iceBulletMgrPtr mIceBulletMgr;

		//damage text manager
		iceDamageTextManager* _damageTextManager;

		//post process manager
		icePostProcessManager* _postProcessManager;

		// level manager
		iceLevelManager* _levelManager;

		std::vector<Ogre::AnimationState*> mPropAnimations;

		//Contadores para dobles pulsaciones de teclas
		Ogre::Real mUpCounter;
		Ogre::Real mDownCounter;
		Ogre::Real mLeftCounter;
		Ogre::Real mRightCounter;

		bool firstUpdate;
};


#endif  
