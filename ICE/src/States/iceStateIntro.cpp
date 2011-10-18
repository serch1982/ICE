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
		mCurrentTime = 0.0;
		_sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

		iceSoundManager* sSound = iceSoundManager::getSingletonPtr();
		sSound->loadIntro();

		mHikariMgr = iceGame::getUI()->getMenu()->mHikariMgr;
		mFlash = mHikariMgr->createFlashOverlay("intro", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
		mFlash->load("intro.swf");
		mFlash->setTransparent(false, true);
		mFlash->bind("finIntro", Hikari::FlashDelegate(this, &iceStateIntro::finIntro));

		sSound->PlaySound(0, Ogre::Vector3::ZERO, 0, 0.5 );
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
	if(evt > 1)
		evt = 1/30;
	mCurrentTime += evt;
	if( mCurrentTime > 59 )
		terminaIntro();
	else
		mHikariMgr->update();
}

Hikari::FlashValue iceStateIntro::finIntro(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	terminaIntro();
	return FLASH_VOID;
}

void iceStateIntro::terminaIntro(){
	iceSoundManager* sSound = iceSoundManager::getSingletonPtr();
	sSound->StopAllSounds();
	sSound->unloadIntro();

	mFlash->stop();
	mFlash->hide();

	//mHikariMgr->destroyFlashControl( mFlash );
	_nextICEStateId = MAINMENU;
}