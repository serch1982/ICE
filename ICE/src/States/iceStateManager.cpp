#include "States\iceStateManager.h"
#include "States\iceStateMenu.h"
#include "States\iceStatePlay.h"
#include "States\iceStatePause.h"
#include "iceGame.h"
#include "Utils\ConfigScriptLoader.h"
#include <Utils\iceLoadingBar.h>
#include <SdkCameraMan.h>

OgreBites::SdkCameraMan* _sdkCameraMan;

iceStateManager::iceStateManager(OIS::InputManager* inputManager):_inputManager(inputManager),
																  _levelToLoad(1),
                                                                  _exit(false) {
    _log = Ogre::LogManager::getSingleton().getLog("iceLog.log");
    _log->logMessage("iceStateManager::iceStateManager()");

    // render window
    _window = iceGame::getRenderWindow();

    // config
    loadResources();
    configureOIS();

    // listeners
    Ogre::Root::getSingleton().addFrameListener(this);
    Ogre::WindowEventUtilities::addWindowEventListener(_window, this);

	//set to load sdk tray screen debug
	iceSdkTray::getInstance()->setupScreenInfo(_window, _mouse, this); 
	iceSdkTray::getInstance()->updateScreenInfo(7, "Solid");

	//load hikari manager
	_hikariMgr = new Hikari::HikariManager(".\\media");


	//set god camera
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	_godCamera = sceneManager->createCamera( "GodCam" );
	_godCamera->setPosition(Ogre::Vector3(0,150,80));
	// Look back along -Z
	_godCamera->lookAt(Ogre::Vector3(0,150,-300));
	_godCamera->setNearClipDistance(5);
	//sdk man
	_sdkCameraMan = new OgreBites::SdkCameraMan(_godCamera); 
	//set the old camera the current
	_oldCamera = iceGame::getCamera();
	// load states
    changeState(new iceStateMenu(this));
}

iceStateManager::~iceStateManager() {
	
	_log->logMessage("iceStateManager::~iceStateManager()");
}

void iceStateManager::finalize(){
 Ogre::WindowEventUtilities::removeWindowEventListener(iceGame::getRenderWindow(), this);
	if( _inputManager )
    {
        _inputManager->destroyInputObject( _mouse );
        _inputManager->destroyInputObject( _keyboard );

        OIS::InputManager::destroyInputSystem(_inputManager);
        _inputManager = 0;
    }
	delete ConfigScriptLoader::getSingletonPtr();
	delete _hikariMgr;
	delete _sdkCameraMan;
	_log->logMessage("iceStateManager::finalize()");
}

void iceStateManager::loadResources() {
    _log->logMessage("iceStateManager::prepareResources()");
	 
	new ConfigScriptLoader();
    Ogre::String groupName, typeName, archiveName;
    Ogre::ConfigFile configFile;

    // open resource file 
    configFile.load("iceResources.cfg");

    // iterator
    Ogre::ConfigFile::SectionIterator sectionIterator = configFile.getSectionIterator();

    while (sectionIterator.hasMoreElements()) {
        // get group name
        groupName = sectionIterator.peekNextKey();

        // get files from section 
        Ogre::ConfigFile::SettingsMultiMap *elements = sectionIterator.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = elements->begin(); i != elements->end(); ++i) {
            typeName = i->first;
            archiveName = i->second;

            // add file
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archiveName,
                                                                           typeName,
                                                                           groupName,
                                                                           true);
        }
    }
	iceLoadingBar loadingBar;
	loadingBar.start(iceGame::getRenderWindow(), 1,1, 0.50);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	loadingBar.finish();
}

void iceStateManager::configureOIS() {
    _log->logMessage("iceStateManager::configureOIS()");

    _keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject(OIS::OISKeyboard, true));
    _mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, true));

    // OIS event callbacks
    _mouse->setEventCallback(this);
    _keyboard->setEventCallback(this);

    //
    _keyboard->setTextTranslation(OIS::Keyboard::Unicode);

    //
    unsigned int width, height, depth;
    int top, left;
    _window->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &mouseState = _mouse->getMouseState();
    mouseState.width = width;
    mouseState.height = height;
}

void iceStateManager::exitGame() {
    _log->logMessage("iceStateManager::exitGame()");
    _exit = true;
}


iceState* iceStateManager::getICEStateByID(const ICEStateId stateName) {
	int sid = stateName;
	switch(sid) {
	case (int)MainMenu:
		_log->logMessage("iceStateManager::changeState() -> new state:  MainMenu" );
        return new iceStateMenu(this);
	case (int)Play:
		_log->logMessage("iceStateManager::changeState() -> new state:  Play" );
        return new iceStatePlay(this);
	case (int)Pause:
		_log->logMessage("iceStateManager::changeState() -> new state:  Pause" );
        return new iceStatePause(this);
	default:
		_log->logMessage("iceStateManager::changeState() -> new state:  default" );
        return new iceStateMenu(this);
	}
}

void iceStateManager::changeState(iceState* icestate) {
    _log->logMessage("iceStateManager::changeState() -> the state is changing");

	_currentState = icestate;
	_currentState->load();
}

bool iceStateManager::keyPressed(const OIS::KeyEvent &arg) {
	if (arg.key == OIS::KC_ESCAPE)
    {
        exitGame();
    }else if (arg.key == OIS::KC_F3)   //show chivatos and stats
    {
		iceSdkTray::getInstance()->showInfo();
		iceSdkTray::getInstance()->showFrameStats();
    }else if (arg.key == OIS::KC_F4)   //show chivatos and stats
    {
        iceSdkTray::getInstance()->hideInfo();
		iceSdkTray::getInstance()->hideFrameStats();
    }
    else if (arg.key == OIS::KC_F5)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (iceGame::getCamera()->getPolygonMode())
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

        iceGame::getCamera()->setPolygonMode(pm);		
		iceSdkTray::getInstance()->updateScreenInfo(7, newVal);
    }
    else if(arg.key == OIS::KC_F6)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        iceGame::getRenderWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
    }else if (arg.key == OIS::KC_F1)
    {
		iceGame::setCamera(_godCamera);
    }else if (arg.key == OIS::KC_F2)
    {
		iceGame::setCamera(_oldCamera);
    }
	_sdkCameraMan->injectKeyDown(arg);
    return this->_currentState->keyPressed(arg);
}

bool iceStateManager::keyReleased(const OIS::KeyEvent &arg) {
	_sdkCameraMan->injectKeyUp(arg);
    return this->_currentState->keyReleased(arg);
}

bool iceStateManager::mouseMoved(const OIS::MouseEvent &arg) {
	_sdkCameraMan->injectMouseMove(arg);
    return this->_currentState->mouseMoved(arg);
}

bool iceStateManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

    return this->_currentState->mousePressed(arg, id);
}

bool iceStateManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

    return this->_currentState->mouseReleased(arg, id);
}

bool iceStateManager::frameStarted(const Ogre::FrameEvent& evt) {
    return true;
}


bool iceStateManager::frameEnded(const Ogre::FrameEvent& evt) {return true;}

bool iceStateManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	// update IOS
    _keyboard->capture();
    _mouse->capture();

	if(_exit == true){
		return false;
	}

	if( _currentState->getNextStateId() == Exit){
		_exit = true;
		return false;
	}

	//------------------ MAIN State manager 
	ICEStateId currentStateId = _currentState->getStateId();
	ICEStateId nextStateId =_currentState->getNextStateId();

	if(currentStateId != nextStateId)
	{
		if((currentStateId == Play) && (nextStateId == Pause)){
			_idleState = _currentState;
			_currentState = getICEStateByID(nextStateId);
			_currentState->load();
		}else if((currentStateId == Pause) && (nextStateId == Play)){
			_currentState->clear();
			_currentState = _idleState;
			_currentState->setNextStateId(Play);
			_idleState = NULL;
			//force to hide cursor
			iceSdkTray::getInstance()->hideCursor();
		}else{
			_currentState->clear();
			_currentState = getICEStateByID(nextStateId);
			_currentState->load();
		}
		
	}
	iceSdkTray::getInstance()->updateFrameEvent(evt);
	_sdkCameraMan->frameRenderingQueued(evt);
	_currentState->update(evt.timeSinceLastFrame);
	//----------------------
	return true;
}

bool iceStateManager::windowClosing(Ogre::RenderWindow* window) {
    _exit = true;
    return true;
}

void iceStateManager::setPathRoot(std::string path){
	_pathRoot = path;
}
std::string iceStateManager::getPathRoot()
{
	return _pathRoot;
}

void iceStateManager::setLevelToLoad(int levelID){
	_levelToLoad = levelID;
}

void iceStateManager::nextLevelToLoad(){
	_levelToLoad++;
}