#ifndef ICESTATEPLAY_H_
#define ICESTATEPLAY_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "iceState.h"
#include "Level\iceLevel.h"
#include "Utils\iceSdkTray.h"
#include "Entities\icePlayer.h"
#include "Entities\iceEnemy.h"
#include "Hikari.h"
#include "../Logic/icePhisics.h"

class iceStateManager;
class iceSoundManager;
class iceLevel;


class iceStatePlay: public iceState{
    public:
        /**
         *  Constructor play state
         */
        iceStatePlay(iceStateManager* stateManager,
			iceSoundManager* soundManager);

        /**
         * Destructor
         */
        ~iceStatePlay();
        
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
         *  get the current icestateid
         */
		void setLevelToLoad(int levelID);
    private:
		//HUD
		Hikari::FlashControl  *_hikariHUD;

		//player
        icePlayer* _player;
		//Enemies
		std::vector<iceEnemy*> _mEnemies;
		 // inverse iterator
        std::vector<iceEnemy*>::const_reverse_iterator _revit_mEnemies;

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

		//phisics
		icePhisics mPhisics;

		//Sound Manager
		iceSoundManager* mSoundManager;
};


#endif  
