#include <States\iceStateStats.h>
#include <Sound\iceSoundManager.h>

// Constructor
iceStateStats::iceStateStats(
	iceSoundManager* soundManager,
	iceLevelManager* levelManager,
	Hikari::HikariManager *hikariManager
	)
	:iceState( soundManager,levelManager, hikariManager)
{
	_log->logMessage("iceStateStats::~iceStateStats()");
	_nextICEStateId = STATS;
}

// Destructor
iceStateStats::~iceStateStats(){
	_log->logMessage("iceStateStats::~iceStateStats()");
}

/**
 *  keypressed event manager
 */
bool iceStateStats::keyPressed(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  keyReleased event manager
 */
bool iceStateStats::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
    
/**
 *  mouseMoved event manager
 */
bool iceStateStats::mouseMoved(const OIS::MouseEvent &arg){
	return true;
}

/**
 *  mousePressed event manager
 */
bool iceStateStats::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

/**
 *  mouseReleased event manager
 */
bool iceStateStats::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateStats::getStateId(){
	return STATS;
}

/** THE UPDATE FUNCTION **/
void iceStateStats::update(Ogre::Real evt){}
