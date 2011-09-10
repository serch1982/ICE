
#ifndef ICESTATE_H_
#define ICESTATE_H_


#include <OIS/OIS.h>
#include <OGRE/Ogre.h>

#include "Sound\iceSoundManager.h"

/**
*  States from the game
*/
enum ICEStateId
{
	INTRO			= 0,
	MAINMENU		= 1,
	PLAY			= 2,
	SHIPSELECTION	= 3,
	LEVELSELECTION  = 4,
	GAMEOVER		= 5,
	PAUSE			= 6,
	LOADLEVEL       = 7,
	CREDITS			= 8,
	OUTRO   		= 9,
	STATS			= 10,
	EXIT			= 11 
};

class iceState {
    public:
        /**
         *  Constructor
         *
         *  @param stateManager 
         *
         *  create the state and elements
         */
        iceState(iceSoundManager* soundManager);

        /**
         *  Destructor
         */
        virtual ~iceState(){}
       
        /**
         *  load elements
         */
        virtual void load(){}

		/**
         *  load elements
         */
        virtual void load(int levelid){}

        /**
         *  Libera la memoria de los elementos del estado.
         */
        virtual void clear(){}

        /**
         *  update state 
         */
        virtual void update(Ogre::Real evt) = 0;
       
        /**
         *  keyPressed event manager
         */
        virtual bool keyPressed(const OIS::KeyEvent &arg) {return true;}
        
        /**
         *  keyReleased event manager
         */
        virtual bool keyReleased(const OIS::KeyEvent &arg) {return true;}
        
        /**
         *  mouseMoved event manager
         */
        virtual bool mouseMoved(const OIS::MouseEvent &arg) {return true;}
        
        /**
         *  mousePressed event manager
         */
        virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;}
        
        /**
         *  mouseReleased event manager
         */
        virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;}


		/** Get ice state Id */
		virtual ICEStateId getStateId() = 0;
		/** Get NEXT icestate Id */
		ICEStateId getNextStateId() { return _nextICEStateId; }
		/** set NEXT icestate Id */
		void setNextStateId( ICEStateId nextICEStateId) { _nextICEStateId = nextICEStateId; }
    protected:
		/** Next Game State Id */
		ICEStateId _nextICEStateId;

        // Log
        Ogre::Log* _log;

		//
        Ogre::SceneManager* _sceneManager;
        bool _loaded;

		//managers
		//sound
		iceSoundManager* _soundManager;
};


#endif  
