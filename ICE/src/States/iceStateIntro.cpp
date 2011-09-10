#include <States\iceStateIntro.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateIntro::iceStateIntro(
	iceSoundManager* soundManager
	)
	:iceState( soundManager)
{
	_log->logMessage("iceStateIntro::iceStateIntro()");
	_nextICEStateId = INTRO;
}

// Destructor
iceStateIntro::~iceStateIntro(){
	_log->logMessage("iceStateIntro::~iceStateIntro()");
}

/**
 *  keypressed event manager
 */
bool iceStateIntro::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateIntro::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateIntro::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateIntro::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateIntro::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateIntro::getStateId(){
	return INTRO;
}

void iceStateIntro::update(Ogre::Real evt){
	
}