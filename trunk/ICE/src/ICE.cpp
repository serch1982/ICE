#include "ICE.h"
#include "iceState.h"


//-------------------------------------------------------------------------------------
ICE::ICE(void)
{

}
//-------------------------------------------------------------------------------------
ICE::~ICE(void)
{
	for(unsigned int i=0;i<mPhases.size();i++)
		delete mPhases[i];
}

bool ICE::setup()
{
	if( CORE::setup() ){
		
		// Creating de Game Log
		mGameLog = Ogre::LogManager::getSingleton().createLog("iceLog.log", false, false, false );
		mGameLog->logMessage( "Sistema: Log creado" );

		// Setup the Ice Menu
		if(!mIceMenu->instance()->setupHikari(".\\media", "menu.swf", mCurrentCamera->getViewport()))
			return false;

		mPhases.resize(3);
		mPhases[0] = new icePhase1();
		mPhases[1] = new icePhase2();
		mPhases[2] = new icePhase3();

		m_iCurrentPhase = 0;

		//load lua logic ----> temporally location
		std::string path = std::string(pathRoot) + "\\logiclua.lua";
		iceLogicLua::instance()->RunFile(path.c_str());  
	}

	return true;
}

//-------------------------------------------------------------------------------------
void ICE::createScene(void)
{
	//Putting elements in the scene
	mPlayer.initialize(mSceneMgr,mSceneMgr->getRootSceneNode()->createChildSceneNode("icePlayer"));
	//mPlayer.setCamera(mGodCam);

	//Loading the level
	mPhases[m_iCurrentPhase]->createScene( mSceneMgr, &mPlayer );

	setCurrentCamera( mPlayer.getCamera() );
}

//Change de current camera by p_pNewCamera
void ICE::setCurrentCamera( Ogre::Camera* p_pNewCamera ){
	mCurrentCamera = p_pNewCamera;
	mWindow->getViewport(0)->setCamera(mCurrentCamera);
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
			ShowCursor(false);
			update(evt.timeSinceLastFrame);
			break;
		case iceState::PAUSE:
			mIceMenu->instance()->update();
			break;
		case iceState::CONTINUE:
			//The main idea is to come back playing from a PAUSE state
			iceState::getInstance()->setState( iceState::PLAY );
			break;
		case iceState::NEXT_LEVEL:
			// Load resources from a level and change state to play
			m_iCurrentPhase++;
			iceState::getInstance()->setState( iceState::LOAD_LEVEL );
			break;
		case iceState::LOAD_LEVEL:
			// Load resources from a level and change state to play
			mSceneMgr->clearScene();
			createScene();
			iceState::getInstance()->setState( iceState::PLAY );
			break;
		case iceState::GAME_OVER:
			// Ending Condition.
			break;
		case iceState::GOD:
			// God mode
			mCameraMan->frameRenderingQueued(evt);
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
	mPhases[m_iCurrentPhase]->update(p_timeSinceLastFrame);
	mPlayer.update(p_timeSinceLastFrame);
}

bool ICE::keyPressed( const OIS::KeyEvent &arg ){
	
	if( !CORE::keyPressed(arg) )
		return false;

	//Changing cameras
	if (arg.key == OIS::KC_0){
		//GodCam
		iceState::getInstance()->setState( iceState::GOD );
		setCurrentCamera( mGodCam );
	}else if (arg.key == OIS::KC_1){
		//PlayerCam
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPlayer.getCamera() );
	}else if (arg.key == OIS::KC_2){
		// Level Cam 0
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 0 ) );
	}else if (arg.key == OIS::KC_3){
		// Level Cam 1
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 1 ) );
	}else if (arg.key == OIS::KC_4){
		// Level Cam 2
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 2 ) );
	}else if (arg.key == OIS::KC_5){
		// Level Cam 3
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 3 ) );
	}else if (arg.key == OIS::KC_6){
		// Level Cam 4
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 4 ) );
	}else if (arg.key == OIS::KC_7){
		// Level Cam 5
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 5 ) );
	}else if (arg.key == OIS::KC_8){
		// Level Cam 6
		iceState::getInstance()->setState( iceState::PLAY );
		setCurrentCamera( mPhases[m_iCurrentPhase]->getCamera( 6 ) );
	}else if (arg.key == OIS::KC_Z){
		bool bSkyBox = mSceneMgr->isSkyBoxEnabled();
		mSceneMgr->setSkyBox( !bSkyBox, "cielo", 20000.0f, false, Ogre::Quaternion::IDENTITY, "level1" );
	}else if (arg.key == OIS::KC_P){
		iceState::getInstance()->setState( iceState::PAUSE );
		mIceMenu->instance()->show();
	}else if( arg.key == OIS::KC_L){
		iceDebugScreen::instance()->updateChivato(8,Ogre::StringConverter::toString(  iceLogicLua::instance()->testfunctionlua(100)));
	}else if ( iceState::getInstance()->getState() == iceState::GOD ){
		mCameraMan->injectKeyDown(arg);
	}

	return true;
}

bool ICE::keyReleased( const OIS::KeyEvent &arg )
{
	CORE::keyReleased( arg );

	if ( iceState::getInstance()->getState() == iceState::GOD)
		mCameraMan->injectKeyUp(arg);
    return true;
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
	else if( iceState::getInstance()->getState() == iceState::PLAY )
		if (id==0)	
		{
			mPlayer.shot();
		}
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
	#include <direct.h>
    #define GetCurrentDir _getcwd
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	#include <unistd.h>
    #define GetCurrentDir getcwd
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ICE app;
		
        try {
			char root_path[1024];
			GetCurrentDir(root_path, 1023);
            app.go(root_path);
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

