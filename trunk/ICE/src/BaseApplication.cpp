/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (August 2010)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#include "BaseApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCameras(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
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
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
	for (unsigned int i=0;i<9;i++)
	{
		char camName[16];
		sprintf(camName,"Camera %d",i);
		mCameras.push_back(mSceneMgr->createCamera(camName));
	}

	//GOD CAM
    // Position it at 500 in Z direction
    mCameras[0]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[0]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[0]->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCameras[0]);   // create a default camera controller

	//CAM 2
	// Position it at 500 in Z direction
    mCameras[2]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[2]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[2]->setNearClipDistance(5);
	
	//CAM 3
	// Position it at 500 in Z direction
    mCameras[3]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[3]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[3]->setNearClipDistance(5);
	
	//CAM 4
	// Position it at 500 in Z direction
    mCameras[4]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[4]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[4]->setNearClipDistance(5);
	
	//CAM 5
	// Position it at 500 in Z direction
    mCameras[5]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[5]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[5]->setNearClipDistance(5);
	
	//CAM 6
	// Position it at 500 in Z direction
    mCameras[6]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[6]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[6]->setNearClipDistance(5);
	
	//CAM 7
	// Position it at 500 in Z direction
    mCameras[7]->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameras[7]->lookAt(Ogre::Vector3(0,0,-300));
    mCameras[7]->setNearClipDistance(5);
	
	//CAM 8
	// Position it at 500 in Z direction
    mCameras[8]->setPosition(Ogre::Vector3(400,10,-300));
    // Look back along -Z
    mCameras[8]->lookAt(Ogre::Vector3(400,10,-600));
    mCameras[8]->setNearClipDistance(5);

	mCurrentCamera = mCameras[1];
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
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

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCurrentCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport�
	for (unsigned int i=0;i<9;i++)
	{
		mCameras[i]->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	}
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
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
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
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
bool BaseApplication::setup(void)
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
    createScene(); //-> now the scene most be create into WORLD or LEVEL class 
	// these parameters except the viewport most be into resource or wherever  

	if(!mICEMenu->instance()->setupHikari(".\\media", "menu.swf", mCurrentCamera->getViewport(), 1024, 768)) return false;


    return true;
};
//-------------------------------------------------------------------------------------

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//TODO: CORE STATE MACHINE
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

	//Game state
	switch(mICEMenu->instance()->getState()){
		case ICEMenu::MENU:
			mICEMenu->instance()->update();
			break;
		case ICEMenu::CONTINUE:
			break;
		case ICEMenu::PLAY:
			break;
		case ICEMenu::GOD:
			break;
		case ICEMenu::EXIT:
			return false;
			break;
	}
	//------------------------------------------------------------
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCurrentCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCurrentCamera->getDerivedOrientation().z));
        }
    }

    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;
        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
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
        mDetailsPanel->setParamValue(10, newVal);
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
	else if (arg.key == OIS::KC_0)
	{
		mICEMenu->instance()->setState(ICEMenu::GOD);
		setCurrentCamera(0);
	}
	else if (arg.key == OIS::KC_1)
	{
		mICEMenu->instance()->setState(ICEMenu::PLAY);
		setCurrentCamera(1);
	}
	else if (arg.key == OIS::KC_2)
	{
		setCurrentCamera(2);
	}
	else if (arg.key == OIS::KC_3)
	{
		setCurrentCamera(3);
	}
	else if (arg.key == OIS::KC_4)
	{
		setCurrentCamera(4);
	}
	else if (arg.key == OIS::KC_5)
	{
		setCurrentCamera(5);
	}
	else if (arg.key == OIS::KC_6)
	{
		setCurrentCamera(6);
	}
	else if (arg.key == OIS::KC_7)
	{
		setCurrentCamera(7);
	}
	else if (arg.key == OIS::KC_8)
	{
		setCurrentCamera(8);
	}else if (arg.key == OIS::KC_Z){
		bool bSkyBox = mSceneMgr->isSkyBoxEnabled();
		mSceneMgr->setSkyBox( !bSkyBox, "cielo", 20000.0f, false, Ogre::Quaternion::IDENTITY, "level1" );
	}
    else if (arg.key == OIS::KC_P){
		mICEMenu->instance()->setState(ICEMenu::MENU);
    }else if (mICEMenu->instance()->getState() == ICEMenu::GOD){
		mCameraMan->injectKeyDown(arg);
	}
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	if (mICEMenu->instance()->getState() == ICEMenu::GOD)
		mCameraMan->injectKeyUp(arg); //not here
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
	if (mICEMenu->instance()->getState() == ICEMenu::GOD)
		mCameraMan->injectMouseMove(arg);//not here
	else if(mICEMenu->instance()->getState() == ICEMenu::MENU)
		mICEMenu->instance()->mouseMoved(arg);
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    if (mICEMenu->instance()->getState() == ICEMenu::GOD)
		mCameraMan->injectMouseDown(arg, id);//not here
	else if(mICEMenu->instance()->getState() == ICEMenu::MENU) 
		mICEMenu->instance()->mouseDown(id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    if (mICEMenu->instance()->getState() == ICEMenu::GOD)
		mCameraMan->injectMouseUp(arg, id); //not here
	else if(mICEMenu->instance()->getState() == ICEMenu::MENU) 
		mICEMenu->instance()->mouseUp(id);
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
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

void BaseApplication::setCurrentCamera(unsigned int cameraIndex)
{
	mCurrentCamera = mCameras[cameraIndex];
	mWindow->getViewport(0)->setCamera(mCurrentCamera);
}
