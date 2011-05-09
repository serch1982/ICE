#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../resource.h"
#endif

#include <iostream>
#include <string>
#include <SDL_mixer.h>
#include <SDL.h>

#include "iceGame.h"
#include "States\iceStateManager.h"
#include "Sound\iceMusicManager.h"
#include "Sound\iceSoundManager.h"
#include "Level\iceLevelManager.h"

Ogre::SceneManager* iceGame::_sceneManager = 0;
Ogre::RenderWindow* iceGame::_window = 0;
Ogre::Viewport* iceGame::_viewport = 0;
Ogre::Log* iceGame::_log = 0;
Ogre::Camera* iceGame::_camera = 0;

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

    //init SDL
    if (!initialiseSDL()) {
        exit(1);
    }

    // levels
    _levelManager = new iceLevelManager();

    // states manager
    _stateManager = new iceStateManager(_inputManager);
}

iceGame::~iceGame() {
	finalize();
}

void iceGame::finalize(){
	delete _soundManager;
	delete _musicManager;
	delete _levelManager;
	_stateManager->finalize();
    delete _root;
	_log->logMessage("iceGame::~finalize()");
}

void iceGame::go(char* pathRoot) {
    _log->logMessage("iceGame::go()");
	_stateManager->setPathRoot(std::string(pathRoot));
	
	_root->startRendering();
	
}

bool iceGame::initialiseOgre() {
    // plugins files (for Debug or Release)
#ifdef _DEBUG
    _pluginsCfg = "icePlugins_d.cfg";
    _windowName = "ICE GAME - Debug";
#else
    _pluginsCfg = "icePlugins.cfg";
    _windowName = "ICE GAME";
#endif

    // config and logging files
    _ogreCfg = "ice.cfg";
    _ogreLog = "ice.log";

    // log level
#ifdef _DEBUG
    bool logOutput = true;
#else
    bool logOutput = false;
#endif

    // create ogre log
    _logManager = new Ogre::LogManager();
    _logManager->createLog(_ogreLog, true, false, false);
	_log = Ogre::LogManager::getSingleton().createLog("iceLog.log", false, false, false );

    // ogre root
    _root = new Ogre::Root(_pluginsCfg, _ogreCfg, _ogreLog);

    // sounds manager
    _musicManager = new iceMusicManager();
	_soundManager = new iceSoundManager();

    // ogre config dialog
    if (!_root->restoreConfig() && !_root->showConfigDialog()) {
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

bool iceGame::initialiseSDL() {
	 _log->logMessage("iceGame::initialiseSDL()");
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
		 _log->logMessage("iceGame::initialiseSDL() SDL_Init(SDL_INIT_AUDIO) ERROR");
        return false;

    atexit(SDL_Quit);

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0){
		  _log->logMessage("iceGame::initialiseSDL() Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) ERROR");
        return false;
    }

    Mix_AllocateChannels(32);
    atexit(Mix_CloseAudio);

    return true;
}

void iceGame::configureSceneManager() {
    _log->logMessage("iceGame::configureSceneManager()");
    _sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_FAR, "sceneManager");

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
    _camera->lookAt(Ogre::Vector3(0,150,-300));

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

