#ifndef ICEGAME_H_
#define ICEGAME_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

class iceStateManager;
class iceLevelManager;
class iceSoundManager;

class iceGame {
    public:
        iceGame();
        ~iceGame();
        
        /**
         *  go
         */
        void go(char* pathRoot);
		 /**
         *  finalize objects
         */
		void finalize(void);

        /**
         *  @return SceneManager
         */
        static Ogre::SceneManager* getSceneManager();
        
        /**
         *  @return window app
         */
        static Ogre::RenderWindow* getRenderWindow();
        
        /**
         *  @return current viewport
         */
        static Ogre::Viewport* getViewPort();
        
        /**
         *  @return log 
         */
        static Ogre::Log* getGameLog();
        
        /**
         *  @return current camera
         */
        static Ogre::Camera* getCamera();

        /**
         *  @set current camera
         */
		static void setCamera(Ogre::Camera* p_Camera);    

    private:
		//
        Ogre::Root* _root;
        Ogre::LogManager* _logManager;
		//
        static Ogre::RenderWindow* _window;
        static Ogre::Camera* _camera;
        static Ogre::Viewport* _viewport;
        static Ogre::SceneManager* _sceneManager;
        static Ogre::Log* _log;

        // OIS
        OIS::InputManager* _inputManager;
        
        // states manager
        iceStateManager* _stateManager;
		
		//
        Ogre::String _windowName;
        
        //plugins, logging and config
        Ogre::String _pluginsCfg;
        Ogre::String _ogreLog;
        Ogre::String _ogreCfg;

        // level manager
        iceLevelManager* _levelManager;
        
        //
        bool initialiseOgre();
        bool initialiseOIS();
        
        //configurations
        void configureSceneManager();
        void createCamera();

		//sound manager
		//iceSoundManager* _soundManager;
};

#endif 
