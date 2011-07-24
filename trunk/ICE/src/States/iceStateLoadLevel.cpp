#include <States\iceStateLoadLevel.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateLoadLevel::iceStateLoadLevel(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager
	)
	:iceState( soundManager,levelManager)
{
	_log->logMessage("iceStateLoadLevel::iceStateLoadLevel()");
	_nextICEStateId = LOADLEVEL;
}

// Destructor
iceStateLoadLevel::~iceStateLoadLevel(){
	_log->logMessage("iceStateLoadLevel::~iceStateLoadLevel()");
}

/**
 *  keypressed event manager
 */
bool iceStateLoadLevel::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateLoadLevel::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateLoadLevel::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateLoadLevel::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateLoadLevel::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateLoadLevel::getStateId(){
	return LOADLEVEL;
}

/** THE UPDATE FUNCTION **/
void iceStateLoadLevel::update(Ogre::Real evt){}
