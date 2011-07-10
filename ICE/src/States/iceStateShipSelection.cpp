#include <States\iceStateShipSelection.h>
#include <States\iceStateManager.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateShipSelection::iceStateShipSelection(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState( soundManager,levelManager, hikariManager)
{
	_log->logMessage("iceStateShipSelection::iceStateShipSelection()");
	_nextICEStateId = SHIPSELECTION;
}

// Destructor
iceStateShipSelection::~iceStateShipSelection(){
	_log->logMessage("iceStateShipSelection::~iceStateShipSelection()");
}

/**
 *  keypressed event manager
 */
bool iceStateShipSelection::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateShipSelection::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateShipSelection::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateShipSelection::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateShipSelection::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateShipSelection::getStateId(){
	return SHIPSELECTION;
}

/** THE UPDATE FUNCTION **/
void iceStateShipSelection::update(Ogre::Real evt){}
