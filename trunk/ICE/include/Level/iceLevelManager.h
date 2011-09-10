
#ifndef ICELEVELMANAGER_H_
#define ICELEVELMANAGER_H_

#include <vector>
#include <OGRE/Ogre.h>
#include "Scene\DotSceneLoader.h"

class iceLevel;

class iceLevelManager: public Ogre::Singleton<iceLevelManager> {
    public:
        /**
         *  Constructor
         */
        iceLevelManager();

        /**
         *  Destructor
         */
        ~iceLevelManager();

        /**
         *  @return instance manager
         */
        static iceLevelManager& getSingleton();

        /**
         *  @return instance pointer manager
         */
        static iceLevelManager* getSingletonPtr();

        /**
         *  @param level id
         */
        iceLevel* getIceLevel(const int id);

        /**
         *  @return total f number of levels
         */
        int getNumLevels() const;
		    
		/**
         *  @return DotSceneLoader
         */
        DotSceneLoader* getDotSceneLoader();
    private:
        std::vector<iceLevel*> _levels;
        int _numLevels;
		Ogre::Log* _log;
		DotSceneLoader* _dotSceneLoader;
		
};

#endif  
