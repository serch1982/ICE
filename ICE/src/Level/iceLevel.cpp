#include <iostream>
#include "Level\iceLevel.h"
#include "Level\iceLevelManager.h"
#include "iceGame.h"
#include "States\iceStateManager.h"


iceLevel::iceLevel(const int id, const Ogre::String& name,const Ogre::String& playerTrajName,const Ogre::String& enemiesConfName)
	: _id(id), _name(name),_playerTrajName(playerTrajName),_enemiesConfName(enemiesConfName), _loaded(false) {

}

iceLevel::~iceLevel() {
    if (_loaded) {
        _loaded = false;
	}
}

void iceLevel::load(std::vector<iceEnemy*>& vectorEnemies, std::vector<iceCutScene*>& vectorCutScenes, iceSoundManager* soundManager) {
    if (!_loaded) {
        _loaded = true;
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(_name);
        
		// Loading a Level
		Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
		Ogre::SceneNode* level = sceneManager->getRootSceneNode()->createChildSceneNode( "root_" + _name );

		iceLevelManager::getSingletonPtr()->getDotSceneLoader()->parseDotScene( _name + ".scene",_name,sceneManager, level, _name + "_" );

		Ogre::SceneNode* staticPhisicObjectsNode = sceneManager->getSceneNode(_name + "_" + "StaticPhisicObjects");
		Ogre::SceneNode* helpersNode = sceneManager->getSceneNode(_name + "_" + "Helpers");
		//staticPhisicObjectsNode->setVisible(false);
		helpersNode->setVisible(false);

		icePlayer::getSingletonPtr()->setTrajectory(new iceLocomotiveTrajectory());
		icePlayer::getSingletonPtr()->getTrajectory()->loadSteps(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getPlayerSteps(),false);

		DotSceneLoader* dsl = iceLevelManager::getSingletonPtr()->getDotSceneLoader();

		std::vector<iceTrajectory*> trajectories = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getTrajectories();

		vectorEnemies = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getEnemies();

		

		if(_id == 1) //level 1
		{
			// light
			iceGame::getSceneManager()->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

			//load sounds
			soundManager->loadLevel1();
			soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);

			vectorCutScenes.push_back(new iceCutScene());
			vectorCutScenes[0]->initialize("startLevel1CutSceneInit","startLevel1CutSceneUpdate",&(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getTrajectories()));
		}
		else //level boss
		{
			// light
			iceGame::getSceneManager()->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

			//load sounds
			soundManager->loadLevelBoss();
			soundManager->PlaySound(0, Ogre::Vector3::ZERO, 0);

			vectorEnemies.push_back(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getMagmaton());
		}

		soundManager->loadSounds();
    }
}


void iceLevel::unload( iceSoundManager* soundManager ) {
    if (_loaded) {
        _loaded = false;

		soundManager->StopAllSounds();

		if( _id == 1 )
			soundManager->unloadLevel1();
		else
			soundManager->unloadLevelBoss();

		soundManager->unloadSounds();

		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(_name);
    }

}

Ogre::TerrainGroup* iceLevel::getTerrain(void){
	return iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getTerrainGroup();
}

bool iceLevel::isLoaded() const {
    return _loaded;
}

std::vector<iceObject*> iceLevel::getSceneObjects()  {
    return iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getObjects();
}

std::vector<iceObject*> iceLevel::getStaticPhisicSceneObjects()  {
    return iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getStaticPhisicObjects();
}

const int iceLevel::getId() const {
    return _id;
}

const Ogre::String& iceLevel::getName() const {
    return _name;
}

void iceLevel::setDebugSceneObjects(bool isDebug)
{
	std::vector<iceObject*> mIceObjects = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getStaticPhisicObjects();
	for(unsigned int i=0;i<mIceObjects.size();i++)
	{
		mIceObjects[i]->setDebugMode(isDebug);
	}
}