
#include "CORE.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


//-------------------------------------------------------------------------------------
CORE::CORE(void)
    : mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCameraMan(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
CORE::~CORE(void)
{
    if (mCameraMan) delete mCameraMan;


    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool CORE::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "ICE Render Window");

        // Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        HWND hwnd;
        mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
        LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
        SetClassLong( hwnd, GCL_HICON, iconID );
#endif
        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void CORE::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void CORE::createCamera(void)
{
    // Create the camera
	mGodCam = mSceneMgr->createCamera( "GodCam" );
	mGodCam->setPosition(Ogre::Vector3(0,150,80));
    // Look back along -Z
    mGodCam->lookAt(Ogre::Vector3(0,150,-300));
    mGodCam->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mGodCam);   // create a default camera controller
	mCurrentCamera = mGodCam;
}
//-------------------------------------------------------------------------------------
void CORE::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	#if defined OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	#elif defined OIS_LINUX_PLATFORM
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	#endif
    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	iceChivatos::instance()->setupChivato(mWindow, mMouse,this,this); 
	iceChivatos::instance()->updateChivato(7, "Solid");

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void CORE::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void CORE::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCurrentCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mGodCam->setAspectRatio( Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void CORE::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void CORE::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void CORE::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void CORE::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "iceResources_d.cfg";
    mPluginsCfg = "icePlugins_d.cfg";
#else
    mResourcesCfg = "iceResources.cfg";
    mPluginsCfg = "icePlugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool CORE::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    createFrameListener();

    // Create the scene
    //createScene(); //-> now the scene most be create into WORLD or LEVEL class 
	// these parameters except the viewport most be into resource or wherever  

    return true;
};

//-------------------------------------------------------------------------------------
bool CORE::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//------------------------------------------------------------
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

	iceChivatos::instance()->updateFrameEvent(evt);
    iceChivatos::instance()->updateChivato( 0, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().x));
    iceChivatos::instance()->updateChivato( 1, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().y));
    iceChivatos::instance()->updateChivato( 2, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().z));
    iceChivatos::instance()->updateChivato( 3, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().w));
    iceChivatos::instance()->updateChivato( 4, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().x));
    iceChivatos::instance()->updateChivato( 5, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().y));
    iceChivatos::instance()->updateChivato( 6, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().z));

    return true;
}
//-------------------------------------------------------------------------------------
bool CORE::keyPressed( const OIS::KeyEvent &arg )
{
    if (iceChivatos::instance()->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   //show chivatos and stats
    {
		iceChivatos::instance()->showChivatos();
		iceChivatos::instance()->showFrameStats();
    }
	if (arg.key == OIS::KC_G)   //show chivatos and stats
    {
        iceChivatos::instance()->hideChivatos();
		iceChivatos::instance()->hideFrameStats();
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCurrentCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCurrentCamera->setPolygonMode(pm);		
		iceChivatos::instance()->updateChivato(7, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    return true;
}

bool CORE::keyReleased( const OIS::KeyEvent &arg )
{
    return true;
}

bool CORE::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

bool CORE::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

bool CORE::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//Adjust mouse clipping area
void CORE::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void CORE::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

/*void CORE::updateCameras(const Ogre::FrameEvent& evt)
{
	Ogre::Vector3 vPos;
	if( mCurrentCamera->getName() == "Camera 3" ){
		vPos = mCurrentCamera->getPosition();
		if( vPos[2] > 770 && deltaCam > 0 )
			deltaCam = -deltaCam;
		else if( vPos[2] < -500 && deltaCam < 0 )
			deltaCam = -deltaCam;
		mCurrentCamera->move( Ogre::Vector3(0.0f,0.0f,deltaCam * evt.timeSinceLastFrame ));

	}else if( mCurrentCamera->getName() == "Camera 2" ){
		vPos = mCurrentCamera->getPosition();
		if( vPos[0] > 320 && deltaCenitalCam > 0 )
			deltaCenitalCam = -deltaCenitalCam;
		else if( vPos[0] < -260 && deltaCenitalCam < 0 )
			deltaCenitalCam = -deltaCenitalCam;
		mCurrentCamera->move( Ogre::Vector3(deltaCenitalCam * evt.timeSinceLastFrame,0.0f,0.0f ));
	}
}*/