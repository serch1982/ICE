#ifndef ICESTATEPAUSE_H_
#define ICESTATEPAUSE_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "iceState.h"
#include "Utils\iceSdkTray.h"
#include <Sound\iceSoundManager.h>
#include "UI/iceMenu.h"

class iceStatePause: public iceState{
    public:
        /**
         *  Constructor menu state pause 
         */
        iceStatePause(
			iceSoundManager* soundManager
			);

        /**
         * Destructor
         */
        ~iceStatePause();
        
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
    private:
		//menu
		iceMenu* mMenu;
};


#endif  
