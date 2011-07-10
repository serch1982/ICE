#include <States\iceStateOutro.h>
#include <States\iceStateManager.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateOutro::iceStateOutro(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState( soundManager,levelManager, hikariManager)
{
	_log->logMessage("iceStateOutro::iceStateOutro()");
	_nextICEStateId = OUTRO;
}

// Destructor
iceStateOutro::~iceStateOutro(){
	_log->logMessage("iceStateOutro::~iceStateOutro()");
}

/**
 *  keypressed event manager
 */
bool iceStateOutro::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateOutro::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateOutro::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateOutro::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateOutro::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateOutro::getStateId(){
	return OUTRO;
}

/** THE UPDATE FUNCTION **/
void iceStateOutro::update(Ogre::Real evt){}
