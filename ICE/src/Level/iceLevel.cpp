#include <iostream>
#include "Level\iceLevel.h"
#include "Level\iceLevelManager.h"
#include "iceGame.h"
#include "States\iceStateManager.h"


iceLevel::iceLevel(const int id, const Ogre::String& name,const Ogre::String& playerTrajName,const Ogre::String& enemiesConfName)
	: _id(id), _name(name),_playerTrajName(playerTrajName),_enemiesConfName(enemiesConfName), _loaded(false) {

}

iceLevel::~iceLevel() {
    unload();
}

void iceLevel::load(icePlayer& player, std::vector<iceEnemy*>& vectorEnemies) {
    if (!_loaded) {
        _loaded = true;
        
		// Loading a Level
		Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
		Ogre::SceneNode* level = sceneManager->getRootSceneNode()->createChildSceneNode( "root_" + _name );

		iceLevelManager::getSingletonPtr()->getDotSceneLoader()->parseDotScene( _name + ".scene",player,_name,sceneManager, level, _name + "_" );

		////set the skybox
		//sceneManager->setSkyBox(true, "cielo", 20000.0f, true, Ogre::Quaternion::IDENTITY ); 

		//// Set ambient light
		//sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

		//// Create a light
		//Ogre::Light* l = sceneManager->createLight("MainLight");
		//l->setType( Ogre::Light::LT_DIRECTIONAL);
		//l->setDirection( -1, 1, -1 );
		//l->setDiffuseColour(1.0, 1.0, 1.0);
		//l->setSpecularColour(1.0, 1.0, 1.0);

        //// song
		_musicName = "level1.mp3";
		//_song = SongManager::getSingleton().load(_musicName);

		player.setTrajectory(new iceLocomotiveTrajectory());
		//player.getTrajectory()->loadSteps(steps,false);
		player.getTrajectory()->loadSteps(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getPlayerSteps(),false);

		//set enemies
		//rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "phase1Enemies");
		//loadEnemies(rootNode, player, vectorEnemies);

		vectorEnemies = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getEnemies();
    }
}

void iceLevel::unload() {
    if (_loaded) {
        _loaded = false;
    }
}

bool iceLevel::isLoaded() const {
    return _loaded;
}

const int iceLevel::getId() const {
    return _id;
}

const Ogre::String& iceLevel::getName() const {
    return _name;
}

const Ogre::String& iceLevel::getMusicName() const{
	return _musicName;
}
