#include <States\iceStateLevelSelection.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateLevelSelection::iceStateLevelSelection(
	iceSoundManager* soundManager
	)
	:iceState( soundManager)
{
	_log->logMessage("iceStateLevelSelection::iceStateLevelSelection()");
	_nextICEStateId = LEVELSELECTION;
}

// Destructor
iceStateLevelSelection::~iceStateLevelSelection(){
	_log->logMessage("iceStateLevelSelection::~iceStateLevelSelection()");
}

/**
 *  keypressed event manager
 */
bool iceStateLevelSelection::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateLevelSelection::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateLevelSelection::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateLevelSelection::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateLevelSelection::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateLevelSelection::getStateId(){
	return LEVELSELECTION;
}

/** THE UPDATE FUNCTION **/
void iceStateLevelSelection::update(Ogre::Real evt){}
