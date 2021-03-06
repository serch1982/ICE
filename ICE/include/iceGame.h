#ifndef ICEGAME_H_
#define ICEGAME_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <States\iceStateManager.h>
#include <Sound\iceSoundManager.h>
#include <UI/iceUI.h>


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

		 /**
         *  create root camera
         */
		static void createCamera();

		static iceUI* getUI();

		static iceStateManager* getStateManager();

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
		static iceUI* mUI;

		//
        Ogre::String _windowName;
        
        //plugins, logging and config
        Ogre::String _pluginsCfg;
        Ogre::String _ogreLog;
        Ogre::String _ogreCfg;

        //
        bool initialiseOgre();
        bool initialiseOIS();
        
        //configurations
        void configureSceneManager();

		// MANAGERS
        // OIS
        OIS::InputManager* _inputManager;
        // states manager
        static iceStateManager* _stateManager;
		//sound manager
		iceSoundManager* _soundManager;
};

#endif 
