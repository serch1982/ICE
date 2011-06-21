
#ifndef ICELEVEL_H_
#define ICELEVEL_H_

#include <vector>
#include <OGRE/Ogre.h>
#include <Terrain/OgreTerrainGroup.h>
#include "Entities\icePlayer.h"
#include "Entities\iceEnemy.h"
#include "Entities\iceObject.h"
#include "Trajectory\iceLocomotiveTrajectory.h"

class icePlayer;
class iceEnemy;

class iceLevel {
    public:
        /**
         *  Constructor
         *
         *  @param level id.
		 *  @param level name.
		 *  @param name of the config node for the player trajectory (config .object file).
		 *  @param name of the config node for enemies (config .object file).
         */
        iceLevel(const int id, const Ogre::String& name,const Ogre::String& playerTrajName,const Ogre::String& enemiesConfName);

        /**
         *  destructor
         */
        ~iceLevel();

        /**
         *  Load level and elements
         */
        void load(icePlayer& player, std::vector<iceEnemy*>& vectorEnemies);

        /**
         *  unload level
         */
        void unload();

        /**
         *  @return is t level is loaded
         */
        bool isLoaded() const;

        /**
         *  @return get level id
         */
        const int getId() const;

        /**
         *  @return level name
         */
        const Ogre::String& getName() const;

		/**
         *  @return get all the objects loaded in the scene
         */
		std::vector<iceObject*> iceLevel::getSceneObjects();

		/**
         *  @return get the terraingroup
         */
		Ogre::TerrainGroup* getTerrain(void);
    private:
        //level
		int _id;
        Ogre::String _name;
		//player and enemies config
		Ogre::String _playerTrajName;
		Ogre::String _enemiesConfName;

        bool _loaded;
};

#endif  
