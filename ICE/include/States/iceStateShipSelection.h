#ifndef _ICE_STATE_SHIP_SELECTION_H__
#define _ICE_STATE_SHIP_SELECTION_H__

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "iceState.h"
#include <Sound\iceSoundManager.h>

class iceStateShipSelection: public iceState{

public:
	iceStateShipSelection(
				iceSoundManager* soundManager
				);
	~iceStateShipSelection();

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

	/** THE UPDATE FUNCTION **/
	void update(Ogre::Real evt);
};

#endif
