#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../resource.h"
#include <Windows.h>
#endif

#include <iostream>
#include <string>

#include "iceGame.h"
#include "States\iceStateManager.h"
#include "Level\iceLevelManager.h"
#include "Sound\iceSoundManager.h"

Ogre::SceneManager* iceGame::_sceneManager = 0;
Ogre::RenderWindow* iceGame::_window = 0;
Ogre::Viewport* iceGame::_viewport = 0;
Ogre::Log* iceGame::_log = 0;
Ogre::Camera* iceGame::_camera = 0;
iceUI* iceGame::mUI = 0;
iceStateManager* iceGame::_stateManager = 0;

iceGame::iceGame() {
    // init Ogre
    if(!initialiseOgre()) {
        exit(1);
    }

    configureSceneManager();
    createCamera();

    // init OIS
    if(!initialiseOIS()) {
        exit(1);
    }

	//load windows icon
	#ifdef WIN32
        HWND hwnd;
        _window->getCustomAttribute("WINDOW", &hwnd);
        HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);
        SetClassLong (hwnd, GCL_HICON, (LONG)LoadIcon (hInst, MAKEINTRESOURCE (IDI_ICON1)));
	#endif

	//UI
	mUI = new iceUI();

	// sound manager
	_soundManager = new iceSoundManager();
	_soundManager->Initialize();

    // states manager
	_stateManager = new iceStateManager(_inputManager, _soundManager->getSingletonPtr() );
}

iceGame::~iceGame() {
	finalize();
}

void iceGame::finalize(){
	
	//Destroy Managers
	//state
	_stateManager->finalize();
	_stateManager = NULL;

	//UI
	delete mUI;
	mUI = NULL;

    //sound
	delete _soundManager;
	_soundManager = NULL;

	OGRE_DELETE _root;
	_root = NULL;

	_log->logMessage("iceGame::~finalize()");
	delete _log;
}

void iceGame::go(char* pathRoot) {
    _log->logMessage("iceGame::go()");
	_stateManager->setPathRoot(std::string(pathRoot));
	
	_root->startRendering();
	
}

bool iceGame::initialiseOgre() {
    // plugins files (for Debug or Release)
#ifdef _DEBUG
    _pluginsCfg = "./configuration/icePlugins_d.cfg";
    _windowName = "ICE GAME - Debug";
#else
    _pluginsCfg = "./configuration/icePlugins.cfg";
    _windowName = "ICE GAME";
#endif

    //config and logging files
    _ogreCfg = "./configuration/ice.cfg";
    _ogreLog = "./logs/ice.log";

    // log level
#ifdef _DEBUG
    bool logOutput = true;
#else
    bool logOutput = false;
#endif

    // create ogre log
    _logManager = new Ogre::LogManager();
    _logManager->createLog(_ogreLog, true, false, false);
	_log = Ogre::LogManager::getSingleton().createLog("./logs/iceLog.log", false, false, false );

    // ogre root
    _root = new Ogre::Root(_pluginsCfg, _ogreCfg, _ogreLog); 

    // ogre config dialog
    if (!_root->showConfigDialog()) {
	// load config from _ogreCfg
	//if (!_root->restoreConfig() && !_root->showConfigDialog()) {
        _log->logMessage("iceGame::initialiseOgre() -> the config dialog was cancelled");
        return false;
    }

    // init Ogre and create window
    if (!_root->initialise(true, _windowName)) {
        _log->logMessage("iceGame::initialiseOgre() ->cannot init ogre");
        return false;
    }

    _window = _root->getAutoCreatedWindow();

    return true;
}

bool iceGame::initialiseOIS() {
    _log->logMessage("iceGame::initialiseOIS()");

    // config keyboard listener OIS
    OIS::ParamList paramList;
    size_t windowHandle = 0;
    std::ostringstream windowHandleStr;
		
    _window->getCustomAttribute("WINDOW", &windowHandle);
    windowHandleStr << windowHandle;
    paramList.insert(std::make_pair(Ogre::String("WINDOW"), windowHandleStr.str()));

    // input manager
    _inputManager = OIS::InputManager::createInputSystem(paramList);
    return true;
}

void iceGame::configureSceneManager() {
    _log->logMessage("iceGame::configureSceneManager()");
	_sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, "sceneManager");

    //shodow technique
    _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
}

void iceGame::createCamera() {
    _log->logMessage("iceGame::createCamera()");

    //config principal camera
    _camera = _sceneManager->createCamera("camera");
    _camera->setNearClipDistance(1);
    //_camera->setFarClipDistance(1000);
    _camera->setPosition(Ogre::Vector3(0,0,0));
    _camera->lookAt(Ogre::Vector3(0,0,-300));

	_window->removeViewport(0);
	_viewport = _window->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    _camera->setAspectRatio(Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));
}

Ogre::SceneManager* iceGame::getSceneManager() {
    return _sceneManager;
}

Ogre::RenderWindow* iceGame::getRenderWindow() {
    return _window;
}

Ogre::Viewport* iceGame::getViewPort() {
    return _viewport;
}

Ogre::Log* iceGame::getGameLog() {
    return _log;
}

Ogre::Camera* iceGame::getCamera() {
    return _camera;
}

void iceGame::setCamera(Ogre::Camera* p_Camera)
{
	iceGame::_camera = p_Camera;
	Ogre::RenderWindow* rw = iceGame::getRenderWindow();
	rw->getViewport(0)->setCamera(iceGame::_camera);
}

iceUI* iceGame::getUI()
{
	return mUI;
}
iceStateManager* iceGame::getStateManager()
{
	return _stateManager;
}
