#ifndef ICESTATEMANAGER_H_
#define ICESTATEMANAGER_H_

#include <vector>
#include <queue>
#include <States\iceState.h>
#include <Sound\iceSoundManager.h>
#include <Level\iceLevelManager.h>
//#include <iceGame.h>

class iceStateManager : public Ogre::FrameListener,
                     public Ogre::WindowEventListener,
                     public OIS::KeyListener,
                     public OIS::MouseListener {
    public:
        /**
         *  Constructor
         */
        iceStateManager(OIS::InputManager* inputManager,
						iceSoundManager* soundManager
						);

        /**
         *  Destructor
         */
        ~iceStateManager();
        
        /**
         * exit loop
         */
        void exitGame();
        
        /**
         *  change the current icestate
         */
        void changeState(iceState* icestate);

        /**
         *  change 
         */
        iceState* getICEStateByID(const ICEStateId icestate);

        /**
         *  get current mouse OIS
         */
        OIS::Mouse* getMouse();

        /**
         *  get current keyboad OIS
         */
        OIS::Keyboard* getKeyboard();
        
        /**
         *  keyPressed event manager
         */
        bool keyPressed(const OIS::KeyEvent &arg);
        
        /**
         *  keyReleased event manager
         */
        bool keyReleased(const OIS::KeyEvent &arg);

        /**
         *  mouseMoved event manager
         */
        bool mouseMoved(const OIS::MouseEvent &arg);

        /**
         *  mousePressed event manager
         */
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /**
         *  mouseReleased event manager
         */
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
       
        /**
         *  fire event started frame
         */
        bool frameStarted(const Ogre::FrameEvent& evt);

        /**
         *  fire event ended frame
         */
        bool frameEnded(const Ogre::FrameEvent& evt);

        /**
         *  fire event queued frame
         */
        bool frameRenderingQueued(const Ogre::FrameEvent& evt);
       
        /**
         *  fire event windowClosed
         */
        void windowClosed(Ogre::RenderWindow* window) {};

        /**
         *  fire event windowResized
         */
        void windowResized(Ogre::RenderWindow* window) {};

        /**
         *  fire event windowClosing
         */
        bool windowClosing(Ogre::RenderWindow* window);
        /**
         *  set the path application root
         */
		void setPathRoot(std::string path);
		/**
         *  get the path application root
         */
		std::string getPathRoot();

		/**
         *  set the level to load
         */
		void setLevelToLoad(int levelID);
		/**
         *  next level to load
         */
		void nextLevelToLoad();

		/**
         *  finalize state manager
         */
		void iceStateManager::finalize();
    private:
		// it's a general control to control the level to load
        int _levelToLoad;

		//god camera
		Ogre::Camera* _godCamera;
		Ogre::Camera* _oldCamera;

		//path
		std::string _pathRoot;

        // Ogre
        Ogre::Log* _log;
        Ogre::RenderWindow* _window;

        //OIS
        OIS::InputManager* _inputManager;
        OIS::Keyboard* _keyboard;
        OIS::Mouse* _mouse;
        
        // resources
        Ogre::String _resourcesCfg;

        //config
        void loadResources();
        void configureOIS();
    
        //states
		iceState* _currentState;
		iceState* _idleState;
        
        // exit flag
        bool _exit;
		
		// states container
		std::vector<iceState*> _statesVector;

		// sound manager
		iceSoundManager* _soundManager;
		// level manager
		iceLevelManager* _levelManager;
		// hikari manager
		Hikari::HikariManager* _hikariManager;
};

inline OIS::Mouse* iceStateManager::getMouse() {return _mouse;}
inline OIS::Keyboard* iceStateManager::getKeyboard() {return _keyboard;}

#endif 
