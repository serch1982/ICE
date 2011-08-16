#include "States\iceStateManager.h"
#include "States\iceStateIntro.h"
#include "States\iceStateShipSelection.h"
#include "States\iceStateLevelSelection.h"
#include "States\iceStateLoadLevel.h"
#include "States\iceStateCredits.h"
#include "States\iceStateStats.h"
#include "States\iceStateGameOver.h"
#include "States\iceStateOutro.h"
#include "States\iceStateMenu.h"
#include "States\iceStatePlay.h"
#include "States\iceStatePause.h"
#include "Sound\iceSoundManager.h"
#include "iceGame.h"
#include "Utils\iceLoadingBar.h"
#include "SdkCameraMan.h"

OgreBites::SdkCameraMan* _sdkCameraMan;

iceStateManager::iceStateManager(OIS::InputManager* inputManager,
								 iceSoundManager* soundManager
								 )
	:_inputManager(inputManager),
	 _soundManager(soundManager),
	 _levelToLoad(1),
	 _exit(false)
{
    _log = iceGame::getGameLog();
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

	iceGame::getUI()->init(this);


	//set god camera
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	_godCamera = sceneManager->createCamera( "GodCam" );
	_godCamera->setPosition(Ogre::Vector3(1226,300,0));
	// Look back along -Z
	_godCamera->lookAt(Ogre::Vector3(1700,300,0));
	_godCamera->setNearClipDistance(5);
	//sdk man
	_sdkCameraMan = new OgreBites::SdkCameraMan(_godCamera); 
	//set the old camera the current
	_oldCamera = iceGame::getCamera();
	
	_levelManager = new iceLevelManager();
	// load states
	_statesVector.push_back( new iceStateIntro( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateMenu( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStatePlay( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateShipSelection( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateLevelSelection( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateGameOver( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStatePause( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateLoadLevel( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateCredits( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateOutro( _soundManager, _levelManager) );
	_statesVector.push_back( new iceStateStats( _soundManager, _levelManager) );

	changeState( _statesVector[PLAY] );
    //changeState(new iceStateMenu(this,_soundManager));
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

	delete _sdkCameraMan;
	_sdkCameraMan = NULL;

	for(unsigned int i = 0; i < _statesVector.size(); ++i ){
		delete _statesVector[i];
		_statesVector[i] = NULL;
	}
	_statesVector.clear();

	_log->logMessage("iceStateManager::finalize()");
}

void iceStateManager::loadResources() {
    _log->logMessage("iceStateManager::prepareResources()");
	 
    Ogre::String groupName, typeName, archiveName;
    Ogre::ConfigFile configFile;

    // open resource file 
    configFile.load("./configuration/iceResources.cfg");

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
	loadingBar.start(iceGame::getRenderWindow(), 4,1, 0.98);
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
	std::stringstream msg;
	msg << "iceStateManager::changeState() -> new state: " << (int)stateName;
	_log->logMessage( msg.str() );
	return _statesVector[stateName];
	
	/*int sid = stateName;
	switch(sid) {
	case (int)MainMenu:
		_log->logMessage("iceStateManager::changeState() -> new state:  MainMenu" );
        return _statesVector[MAINMENU];
	case (int)Play:
		_log->logMessage("iceStateManager::changeState() -> new state:  Play" );
        return new iceStatePlay(this,_soundManager);
	case (int)Pause:
		_log->logMessage("iceStateManager::changeState() -> new state:  Pause" );
        return new iceStatePause(this,_soundManager);
	default:
		_log->logMessage("iceStateManager::changeState() -> new state:  default" );
        return new iceStateMenu(this,_soundManager);
	}*/
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
	//iceSoundManager::getSingletonPtr()->FrameStarted( , evt.timeSinceLastFrame );
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

	if( _currentState->getNextStateId() == EXIT){
		_exit = true;
		return false;
	}

	//------------------ MAIN State manager 
	ICEStateId currentStateId = _currentState->getStateId();
	ICEStateId nextStateId =_currentState->getNextStateId();

	if(currentStateId != nextStateId)
	{
		if((currentStateId == PLAY) && (nextStateId == PAUSE)){
			_idleState = _currentState;
			_currentState = getICEStateByID(nextStateId);
			_currentState->load();
		}else if((currentStateId == PAUSE) && (nextStateId == PLAY)){
			_currentState->clear();
			//delete _currentState;
			_currentState = _idleState;
			_currentState->setNextStateId(PLAY);
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
	Ogre::Real timeSinceLastFrame = evt.timeSinceLastFrame;
//#ifdef _DEBUG
//	if(timeSinceLastFrame > 1) timeSinceLastFrame = 1;
//#endif
	iceGame::getUI()->update(timeSinceLastFrame);
	_currentState->update(timeSinceLastFrame);
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

void iceStateManager::menuExitClick()
{
	_currentState->setNextStateId(EXIT);
}

void iceStateManager::menuPlayClick()
{
	_currentState->setNextStateId(PLAY);
}

void iceStateManager::menuContinueClick()
{
	_currentState->setNextStateId(PLAY);
}