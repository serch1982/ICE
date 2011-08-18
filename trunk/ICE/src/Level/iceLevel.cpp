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

void iceLevel::load(std::vector<iceEnemy*>& vectorEnemies, std::vector<iceCutScene*>& vectorCutScenes) {
    if (!_loaded) {
        _loaded = true;
        
		// Loading a Level
		Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
		Ogre::SceneNode* level = sceneManager->getRootSceneNode()->createChildSceneNode( "root_" + _name );

		iceLevelManager::getSingletonPtr()->getDotSceneLoader()->parseDotScene( _name + ".scene",_name,sceneManager, level, _name + "_" );

		Ogre::SceneNode* staticPhisicObjectsNode = sceneManager->getSceneNode(_name + "_" + "StaticPhisicObjects");
		//Ogre::SceneNode* helpersNode = sceneManager->getSceneNode(_name + "_" + "Helpers");
		staticPhisicObjectsNode->setVisible(false);
		//helpersNode->setVisible(false);

		icePlayer::getSingletonPtr()->setTrajectory(new iceLocomotiveTrajectory());
		icePlayer::getSingletonPtr()->getTrajectory()->loadSteps(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getPlayerSteps(),false);

		//std::vector<iceStep> steps1;
		//steps1.push_back(iceStep(Ogre::Vector3(100,0,0),Ogre::Radian(0),0));
		//steps1.push_back(iceStep(Ogre::Vector3(50,0,0),Ogre::Radian(0),20));
		//steps1.push_back(iceStep(Ogre::Vector3(0,0,0),Ogre::Radian(0),30));
		//std::vector<iceStep> steps2;
		//steps2.push_back(iceStep(Ogre::Vector3(-100,0,0),Ogre::Radian(0),0));
		//steps2.push_back(iceStep(Ogre::Vector3(-50,0,0),Ogre::Radian(0),20));
		//steps2.push_back(iceStep(Ogre::Vector3(0,0,0),Ogre::Radian(0),30));
		//std::vector<iceStep> steps0;
		//steps0.push_back(iceStep(Ogre::Vector3(100,0,100),Ogre::Radian(0),0));
		//steps0.push_back(iceStep(Ogre::Vector3(50,0,100),Ogre::Radian(0),20));
		//steps0.push_back(iceStep(Ogre::Vector3(0,0,100),Ogre::Radian(0),30));

		//std::vector<iceTrajectory*> trajectories;
		//trajectories.push_back(new iceLocomotiveTrajectory(steps0));
		//trajectories.push_back(new iceLocomotiveTrajectory(steps1));
		//trajectories.push_back(new iceLocomotiveTrajectory(steps2));

		DotSceneLoader* dsl = iceLevelManager::getSingletonPtr()->getDotSceneLoader();

		std::vector<iceTrajectory*> trajectories = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getTrajectories();

		if(_id == 1) //level 1
		{
			vectorCutScenes.push_back(new iceCutScene());
			vectorCutScenes[0]->initialize("startLevel1CutSceneInit","startLevel1CutSceneUpdate",&(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getTrajectories()));
		}
		else //level boss
		{
		}


		vectorEnemies = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getEnemies();
    }
}

void iceLevel::unload() {
    if (_loaded) {
        _loaded = false;
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
	std::vector<iceObject*> mIceObjects = iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getObjects();
	for(unsigned int i=0;i<mIceObjects.size();i++)
	{
		mIceObjects[i]->setDebugMode(isDebug);
	}
}