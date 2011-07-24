#include <States\iceStateGameOver.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateGameOver::iceStateGameOver(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager
	)
	:iceState( soundManager,levelManager)
{
	_log->logMessage("iceStateGameOver::iceStateGameOver()");
	_nextICEStateId = GAMEOVER;
}

// Destructor
iceStateGameOver::~iceStateGameOver(){
	_log->logMessage("iceStateGameOver::~iceStateGameOver()");
}

/**
 *  keypressed event manager
 */
bool iceStateGameOver::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateGameOver::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateGameOver::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateGameOver::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateGameOver::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateGameOver::getStateId(){
	return GAMEOVER;
}

/** THE UPDATE FUNCTION **/
void iceStateGameOver::update(Ogre::Real evt){}
