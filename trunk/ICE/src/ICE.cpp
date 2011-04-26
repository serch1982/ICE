#include "ICE.h"
#include "iceState.h"


//-------------------------------------------------------------------------------------
ICE::ICE(void)
{
	m_bShooting = false;
	m_bShowPhysics = false;
	m_iCurrentPhase = 0;
}
//-------------------------------------------------------------------------------------
ICE::~ICE(void)
{
	for(unsigned int i=0;i<mPhases.size();i++)
		delete mPhases[i];

	if( mPhysics )
		delete mPhysics;
}

bool ICE::setup()
{
	if( CORE::setup() ){
		
		// Creating de Game Log
		mGameLog = Ogre::LogManager::getSingleton().createLog("iceLog.log", false, false, false );
		mGameLog->logMessage( "Sistema: Log creado" );

		// Setup the Ice Menu
		if(!iceMenu::getInstance()->setupHikari(".\\media", "menu.swf", mCurrentCamera->getViewport()))
			return false;

		mPhases.resize(2);
		mPhases[0] = new icePhase1();
		//mPhases[1] = new icePhase2();
		mPhases[1] = new icePhase3();

		//Set the current phase
		m_iCurrentPhase = 0;

		//load lua logic ----> temporally location
		std::string path = std::string(pathRoot) + "\\logiclua.lua";
		iceLogicLua::getInstance()->setLog();
		iceLogicLua::getInstance()->RunFile(path.c_str());  

		mPhysics = new icePhysicsMgr();
		mPhysics->init( mSceneMgr );
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
	mPhases[m_iCurrentPhase]->initialize();
	mPhases[m_iCurrentPhase]->createScene( mSceneMgr, &mPlayer );

	setCurrentCamera( mPlayer.getCamera() );

	mPhysics->setLevel( m_iCurrentPhase );
	mPhysics->addPlayer( mPlayer );
	//mPhysics->addEnemies( mPhase[m_iCurrentPhase]->getEnemies() );
	//mPhysics->addEnemies( mPhases[m_iCurrentPhase]->getEnemies() );
	//set the HUD
	iceHUD::getInstance()->setupHUD(".\\media", "HUD.swf", mCurrentCamera->getViewport());
}

//Change de current camera by p_pNewCamera
void ICE::setCurrentCamera( Ogre::Camera* p_pNewCamera ){
	mCurrentCamera = p_pNewCamera;
	mWindow->getViewport(0)->setCamera(mCurrentCamera);
}

bool ICE::frameStarted(const Ogre::FrameEvent& evt ){
	if( iceState::getInstance()->getState() == iceState::PLAY ){
		mPhysics->update( evt.timeSinceLastFrame );
	}
	return true;
}

bool ICE::frameEnded(const Ogre::FrameEvent& evt ){
	if( iceState::getInstance()->getState() == iceState::PLAY ){
		mPhysics->update( evt.timeSinceLastFrame );
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

	// Pau * Shoot control
	if (iceState::getInstance()->getState()!=iceState::PLAY){
		m_bShooting = false;
	}
	
	//Game state
	switch( iceState::getInstance()->getState() ){
		case iceState::MENU:
			iceDebugScreen::getInstance()->showCursor();
			// Update the MENU
			iceMenu::getInstance()->update();			
			break;
		case iceState::PLAY:
			// Playing the game
			iceDebugScreen::getInstance()->hideCursor();
			if(mPhases[m_iCurrentPhase]->isPhaseEnded())
				iceState::getInstance()->setState(iceState::NEXT_LEVEL);
			else
				update( evt.timeSinceLastFrame);
			break;
		case iceState::PAUSE:
			iceMenu::getInstance()->update();
			iceHUD::getInstance()->hide();			
			break;
		case iceState::CONTINUE:
			//The main idea is to come back playing from a PAUSE state
			iceState::getInstance()->setState( iceState::PLAY );			
			break;
		case iceState::NEXT_LEVEL:
			m_iCurrentPhase++;
			if(m_iCurrentPhase<=1)
				iceState::getInstance()->setState( iceState::LOAD_LEVEL );
			else
			{
				mPhysics->reset();
				mSceneMgr->clearScene();
				if(!iceMenu::getInstance()->setupHikari(".\\media", "menu.swf", mCurrentCamera->getViewport()))
					return false;
				iceState::getInstance()->setState( iceState::MENU );
			}
			break;
		case iceState::LOAD_LEVEL:
			// Load resources from a level and change state to play
			mPhysics->reset();
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

void ICE::update( Ogre::Real p_timeSinceLastFrame )
{
	mPlayer.update(p_timeSinceLastFrame,m_bShooting);
	mPhases[m_iCurrentPhase]->update(p_timeSinceLastFrame);

	iceHUD::getInstance()->update();
	iceHUD::getInstance()->setLife(100);
	iceHUD::getInstance()->setWeapon("Rocket");
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
		mSceneMgr->setSkyBox( !bSkyBox, "cielo", 20000.0f, true, Ogre::Quaternion::IDENTITY, "level1" );
	}else if (arg.key == OIS::KC_P){
		iceState::getInstance()->setState( iceState::PAUSE );
		iceMenu::getInstance()->show();
	}else if( arg.key == OIS::KC_L){
		m_bShowPhysics = !m_bShowPhysics;
		mPhysics->setShowDebug( m_bShowPhysics );
	}else if( arg.key == OIS::KC_N){
		iceState::getInstance()->setState(iceState::NEXT_LEVEL);
	}else if ( iceState::getInstance()->getState() == iceState::GOD )
	{
		mCameraMan->injectKeyDown(arg);
	}
	else if( arg.key == OIS::KC_W)
	{
		mPlayer.addYUserDeviation(1);
	}
	else if( arg.key == OIS::KC_A)
	{
		mPlayer.addXUserDeviation(-1);
	}
	else if( arg.key == OIS::KC_S)
	{
		mPlayer.addYUserDeviation(-1);
	}
	else if( arg.key == OIS::KC_D)
	{
		mPlayer.addXUserDeviation(1);
	}

	return true;
}

bool ICE::keyReleased( const OIS::KeyEvent &arg )
{
	CORE::keyReleased( arg );

	if ( iceState::getInstance()->getState() == iceState::GOD)
		mCameraMan->injectKeyUp(arg);
	else if( arg.key == OIS::KC_W)
	{
		mPlayer.addYUserDeviation(-1);
	}
	else if( arg.key == OIS::KC_A)
	{
		mPlayer.addXUserDeviation(1);
	}
	else if( arg.key == OIS::KC_S)
	{
		mPlayer.addYUserDeviation(1);
	}
	else if( arg.key == OIS::KC_D)
	{
		mPlayer.addXUserDeviation(-1);
	}
    return true;
}


bool ICE::mouseMoved( const OIS::MouseEvent &arg )
{
	bool ret = CORE::mouseMoved(arg);

	if( iceState::getInstance()->getState() == iceState::GOD ){
		mCameraMan->injectMouseMove(arg);
	}else if( iceState::getInstance()->getState() == iceState::MENU || 
		iceState::getInstance()->getState() == iceState::PAUSE ){
		iceDebugScreen::getInstance()->moveMouse(arg);
		iceMenu::getInstance()->mouseMoved(arg);
	}else if( iceState::getInstance()->getState() == iceState::PLAY ){
		mPlayer.processMouseMoved(arg);
	}
    return ret;
}

bool ICE::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CORE::mousePressed( arg, id );

    if ( iceState::getInstance()->getState() == iceState::GOD){
		mCameraMan->injectMouseDown(arg, id);//not here
	}else if( iceState::getInstance()->getState() == iceState::MENU ||
		iceState::getInstance()->getState() == iceState::PAUSE ){ 
			iceMenu::getInstance()->mouseDown(id);
	}else if( iceState::getInstance()->getState() == iceState::PLAY ){		
		if (id==0)
			m_bShooting = true;
	}
    return true;
}

bool ICE::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CORE::mouseReleased( arg, id );

    if ( iceState::getInstance()->getState() == iceState::GOD){
		mCameraMan->injectMouseUp(arg, id); //not here
	}else if( iceState::getInstance()->getState() == iceState::MENU ||
		iceState::getInstance()->getState() == iceState::PAUSE ){ 
		iceMenu::getInstance()->mouseUp(id);
    }else if( iceState::getInstance()->getState() == iceState::PLAY ){		
		if(id==0)
			m_bShooting = false;
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

