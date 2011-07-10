#include <States\iceStateCredits.h>


// Constructor
iceStateCredits::iceStateCredits(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState( soundManager,levelManager, hikariManager)
{
	_log->logMessage("iceStateCredits::iceStateCredits()");
	_nextICEStateId = CREDITS;
}

// Destructor
iceStateCredits::~iceStateCredits(){
	_log->logMessage("iceStateCredits::~iceStateCredits()");
}

/**
 *  keypressed event manager
 */
bool iceStateCredits::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateCredits::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateCredits::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateCredits::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateCredits::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateCredits::getStateId(){
	return CREDITS;
}

/** THE UPDATE FUNCTION **/
void iceStateCredits::update(Ogre::Real evt){}
