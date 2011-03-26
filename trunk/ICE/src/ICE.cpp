#include "ICE.h"
#include "iceState.h"
#include <OgreLogManager.h>
#include <stdlib.h>

//-------------------------------------------------------------------------------------
ICE::ICE(void)
{
	m_iCurrentLevel = 1;
}
//-------------------------------------------------------------------------------------
ICE::~ICE(void)
{
}

bool ICE::setup()
{
	if( CORE::setup() ){
		
		// Creating de Game Log
		mGameLog = Ogre::LogManager::getSingleton().createLog("iceLog.log", false, false, false );
		mGameLog->logMessage( "Sistema: Log creado" );

		// Setup the Ice Menu
		if(!mIceMenu->instance()->setupHikari(".\\media", "menu.swf", mCurrentCamera->getViewport(), 1024, 768))
			return false;

		// creating the scene
		//createScene();
	}

	return true;
}

//-------------------------------------------------------------------------------------
void ICE::createScene(void)
{
	//Putting elements in the scene
	mPlayer.initialize(mSceneMgr,mSceneMgr->getRootSceneNode()->createChildSceneNode("icePlayer"));
	mPlayer.setCamera(mGodCam);

	//Loading the level
	mLevel.createScene( mSceneMgr, m_iCurrentLevel );

}

bool ICE::keyPressed( const OIS::KeyEvent &arg ){
	
	if( !CORE::keyPressed(arg) )
		return false;

	if (arg.key == OIS::KC_0)
	{
		iceState::getInstance()->setState( iceState::GOD );
		//setCurrentCamera(0);
	}
	else if (arg.key == OIS::KC_1)
	{
		iceState::getInstance()->setState( iceState::PLAY );
		//setCurrentCamera(1);
	}
	else if (arg.key == OIS::KC_2)
	{
		iceState::getInstance()->setState( iceState::PLAY );
		//setCurrentCamera(2);
	}
	else if (arg.key == OIS::KC_3)
	{
		iceState::getInstance()->setState( iceState::PLAY );
		//setCurrentCamera(3);
	}
	else if (arg.key == OIS::KC_4)
	{
		//setCurrentCamera(4);
	}
	else if (arg.key == OIS::KC_5)
	{
		//setCurrentCamera(5);
	}
	else if (arg.key == OIS::KC_6)
	{
		//setCurrentCamera(6);
	}
	else if (arg.key == OIS::KC_7)
	{
		//setCurrentCamera(7);
		iceChivatos::instance()->updateChivato(8,Ogre::StringConverter::toString(  iceLuaLogic::instance()->testFromOgre()));
	}
	else if (arg.key == OIS::KC_8)
	{
		//setCurrentCamera(8);
	}else if (arg.key == OIS::KC_Z){
		bool bSkyBox = mSceneMgr->isSkyBoxEnabled();
		mSceneMgr->setSkyBox( !bSkyBox, "cielo", 20000.0f, false, Ogre::Quaternion::IDENTITY, "level1" );
	}
    else if (arg.key == OIS::KC_P){
		iceState::getInstance()->setState( iceState::PAUSE );
		mIceMenu->instance()->show();
	}else if ( iceState::getInstance()->getState() == iceState::GOD ){
		mCameraMan->injectKeyDown(arg);
	}

	return true;
}

bool ICE::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if( !CORE::frameRenderingQueued(evt))
		return false;
	
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
	
	//Game state
	switch( iceState::getInstance()->getState() ){
		case iceState::MENU:
			// Update the MENU
			mIceMenu->instance()->update();
			break;
		case iceState::PLAY:
			// Playing the game
			update(evt.timeSinceLastFrame);
			break;
		case iceState::PAUSE:
			mIceMenu->instance()->update();
			break;
		case iceState::CONTINUE:
			//The main idea is to come back playing from a PAUSE state
			iceState::getInstance()->setState( iceState::PLAY );
			break;
		case iceState::LOAD_LEVEL:
			mSceneMgr->clearScene();
			createScene();
			iceState::getInstance()->setState( iceState::PLAY );
			//mLevel.createScene(mSceneMgr,m_iCurrentLevel);
			// Load resources from a level and change state to play
			break;
		case iceState::GAME_OVER:
			// Ending Condition.
			break;
		case iceState::GOD:
			// God mode
			break;
		case iceState::EXIT:
			return false;
			break;
		default:
			break;
	}

    return true;
}

void ICE::update( Ogre::Real p_timeSinceLastFrame ){
	mLevel.update(p_timeSinceLastFrame);
	mPlayer.updateShipPosition(p_timeSinceLastFrame);
}

bool ICE::mouseMoved( const OIS::MouseEvent &arg )
{
	bool ret = CORE::mouseMoved(arg);

	if( iceState::getInstance()->getState() == iceState::GOD )
		mCameraMan->injectMouseMove(arg);
	else if( iceState::getInstance()->getState() == iceState::MENU || 
		iceState::getInstance()->getState() == iceState::PAUSE )
		mIceMenu->instance()->mouseMoved(arg);
	else if( iceState::getInstance()->getState() == iceState::PLAY )
		mPlayer.processMouseMoved(arg);
    return ret;
}

bool ICE::keyReleased( const OIS::KeyEvent &arg )
{
	CORE::keyReleased( arg );

	if ( iceState::getInstance()->getState() == iceState::GOD)
		mCameraMan->injectKeyUp(arg); //not here
    return true;
}

bool ICE::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CORE::mousePressed( arg, id );

    if ( iceState::getInstance()->getState() == iceState::GOD)
		mCameraMan->injectMouseDown(arg, id);//not here
	else if( iceState::getInstance()->getState() == iceState::MENU ||
		iceState::getInstance()->getState() == iceState::PAUSE ) 
		mIceMenu->instance()->mouseDown(id);
    return true;
}

bool ICE::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CORE::mouseReleased( arg, id );

    if ( iceState::getInstance()->getState() == iceState::GOD)
		mCameraMan->injectMouseUp(arg, id); //not here
	else if( iceState::getInstance()->getState() == iceState::MENU ||
		iceState::getInstance()->getState() == iceState::PAUSE ) 
		mIceMenu->instance()->mouseUp(id);
    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ICE app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif



/*
	//CAM 2
    mCameras[2]->setPosition(Ogre::Vector3(-260,650,590));
    mCameras[2]->lookAt(Ogre::Vector3(-260,0,590));
    mCameras[2]->setNearClipDistance(5);
	
	//CAM 3
    mCameras[3]->setPosition(Ogre::Vector3(-400,10,-500));
    mCameras[3]->lookAt(Ogre::Vector3(-200,10,-500));
    mCameras[3]->setNearClipDistance(5);
	
	//CAM 4
	mCameras[4]->setPosition(Ogre::Vector3(0,1000,0));
    mCameras[4]->lookAt(Ogre::Vector3(0,0,0));
    mCameras[4]->setNearClipDistance(5);
	mCameras[4]->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	
	//CAM 5
    mCameras[5]->setPosition(Ogre::Vector3(-880,5,612));
    mCameras[5]->lookAt(Ogre::Vector3(-227,10,880));
    mCameras[5]->setNearClipDistance(5);
	
	//CAM 6
    mCameras[6]->setPosition(Ogre::Vector3(492,10,462));
    mCameras[6]->lookAt(Ogre::Vector3(-161,10,218));
    mCameras[6]->setNearClipDistance(5);
	
	//CAM 7
    mCameras[7]->setPosition(Ogre::Vector3(-168,10,684));
    mCameras[7]->lookAt(Ogre::Vector3(-142,10,520));
    mCameras[7]->setNearClipDistance(5);
	
	//CAM 8
    mCameras[8]->setPosition(Ogre::Vector3(302,10,-115));
    mCameras[8]->lookAt(Ogre::Vector3(302,10,-100));
    mCameras[8]->setNearClipDistance(5);*/
