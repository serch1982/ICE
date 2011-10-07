#include <States\iceStateIntro.h>
#include <Sound\iceSoundManager.h>
#include <OgreVideo\OgreVideoManager.h>
#include <OgreVideo\TheoraVideoManager.h>

// Constructor
iceStateIntro::iceStateIntro(
	iceSoundManager* soundManager
	)
	:iceState( soundManager)
{
	_log->logMessage("iceStateIntro::iceStateIntro()");
	_nextICEStateId = INTRO;
	mgr = new Ogre::OgreVideoManager();
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

void iceStateIntro::load() {

	/* VideoCode */
	Ogre::Rectangle2D videoRect;
	// Render the background after everything else
	videoRect.setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );

	// Create background rectangle covering the whole screen
	videoRect.setCorners(-1.0, 1.0, 1.0, -1.0);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	videoRect.setBoundingBox(aabInf);
	videoRect.setMaterial( "intro_material");

/*	Ogre::ManualObject* model = _sceneManager->createManualObject("video_quad");
	model->begin("intro_material");

	float left = -1;
	float right = 1;
	float top = 1;
	float bottom = -1;

	model->position(right,bottom,0); model->textureCoord(1,1);
	model->position(right,top   ,0); model->textureCoord(1,0);
	model->position(left ,top   ,0); model->textureCoord(0,0);
	model->position(left ,top   ,0); model->textureCoord(0,0);
	model->position(right,bottom,0); model->textureCoord(1,1);
	model->position(left, bottom,0); model->textureCoord(0,1);

	model->end();
	// make the model 2D
	model->setUseIdentityProjection(true);
	model->setUseIdentityView(true);
	// and atach it to the root node*/
	Ogre::SceneNode* node = _sceneManager->getRootSceneNode()->createChildSceneNode("video");
	node->setPosition( 0,0,0 );
	node->attachObject( &videoRect );

	//mgr->setInputName("konqi.ogg");
	//mgr->createDefinedTexture("intro_material");

	//TheoraVideoClip *clip = mgr->getVideoClipByName("konqi.ogg");
	//clip->play();

	//mgr->getTheoraVideoManager()->getVideoClipByName("konqi.ogg")->setAutoRestart(1);
}

 /**
 *  get the current icestateid
 */
ICEStateId iceStateIntro::getStateId(){
	return INTRO;
}

void iceStateIntro::update(Ogre::Real evt){
	//if( video->hasEnded() ){
	//	_nextICEStateId = MAINMENU;
	//}
}