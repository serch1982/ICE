#include <States\iceStateIntro.h>
#include <Sound\iceSoundManager.h>
#include <Hikari.h>
#include <iceGame.h>

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
	if( arg.key == OIS::KC_ESCAPE)
	{
		terminaIntro();
	}
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

void iceStateIntro::load(){

	try{
		_sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

		mHikariMgr = iceGame::getUI()->getMenu()->mHikariMgr;
		mFlash = mHikariMgr->createFlashOverlay("intro", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
		mFlash->load("intro.swf");
		mFlash->setTransparent(false, true);
		mFlash->bind("finIntro", Hikari::FlashDelegate(this, &iceStateIntro::finIntro));
		mFlash->show();
		//mFlash->hide();
	}catch(char* ex) {
		iceGame::getGameLog()->logMessage(ex);
	}
}


void iceStateIntro::clear(){
	
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateIntro::getStateId(){
	return INTRO;
}

void iceStateIntro::update(Ogre::Real evt){
	mHikariMgr->update();
}

Hikari::FlashValue iceStateIntro::finIntro(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	terminaIntro();
	return FLASH_VOID;
}

void iceStateIntro::terminaIntro(){
	mFlash->stop();
	mFlash->hide();
	//mHikariMgr->destroyFlashControl( mFlash );
	_nextICEStateId = MAINMENU;
}