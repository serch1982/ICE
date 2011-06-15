#ifndef ICESTATEMENU_H_
#define ICESTATEMENU_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "Hikari.h"
#include "iceState.h"
#include "Utils\iceSdkTray.h"

class iceStateManager;
class iceSoundManager;

class iceStateMenu: public iceState{
    public:
        /**
         *  Constructor menu state
         */
        iceStateMenu(iceStateManager* stateManager,
				iceSoundManager* soundManager);

        /**
         * Destructor
         */
        ~iceStateMenu();
        
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
		//menu flash
		Hikari::FlashControl  *_hikariMenu;
		//menu callbacks to flash
		Hikari::FlashValue menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
		Hikari::FlashValue menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
		Hikari::FlashValue menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		//Sound
		iceSoundManager* mSoundManager;
};


#endif  
