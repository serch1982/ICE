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

		iceLevelManager::getSingletonPtr()->getDotSceneLoader()->parseDotScene( _name + ".scene",_name,sceneManager, level, _name + "_" );

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

		ConfigNode *rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", _playerTrajName);
		//std::vector<iceStep> steps = getStepsFromResources(rootNode);
		player.setTrajectory(new iceLocomotiveTrajectory());
		//player.getTrajectory()->loadSteps(steps,false);
		player.getTrajectory()->loadSteps(iceLevelManager::getSingletonPtr()->getDotSceneLoader()->getPlayerSteps(),false);

		//set enemies
		rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "phase1Enemies");
		loadEnemies(rootNode, player, vectorEnemies);
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

void iceLevel::loadEnemies(ConfigNode* p_EnemiesNode,icePlayer& player, std::vector<iceEnemy*>& vEnemies)
{
	vector<iceStep> steps;
	int enemiesNumber = p_EnemiesNode->findChild("enemiesNumber")->getValueI();

	vEnemies.resize(enemiesNumber);
	for(int i=0;i<enemiesNumber;i++)
	{
		char enemyName[10];
		sprintf(enemyName,"enemy%d",i+1);
		ConfigNode* enemyNode = p_EnemiesNode->findChild(enemyName);

		iceEnemy::ENEMYTYPE enemyType;

		switch(enemyNode->findChild("enemyType")->getValueI())
		{
			case 0:
				enemyType = iceEnemy::MINIMAGMATON;
				break;
			case 1:
				enemyType = iceEnemy::KAMIKAZE;
				break;
			case 2:
				enemyType = iceEnemy::SMART;
				break;
			case 3:
				enemyType = iceEnemy::VOLCANO;
				break;
			case 4:
				enemyType = iceEnemy::MAGMATON;
				break;
			default:
				enemyType = iceEnemy::MINIMAGMATON;
				break;
		}
		Ogre::Real activationTime = enemyNode->findChild("activationTime")->getValueF();
		bool isAttachedToPlayer = false;
		int isAtt = enemyNode->findChild("isAttachedToPlayer")->getValueI();
		if(isAtt > 0) isAttachedToPlayer = true;

		//Trajectory
		ConfigNode* trajectoryNode = enemyNode->findChild("trajectory");
		steps = getStepsFromResources(trajectoryNode);

		vEnemies[i] = new iceEnemy();
		vEnemies[i]->initialize(i, &player ,activationTime,enemyType,isAttachedToPlayer);
		vEnemies[i]->setTrajectory(new iceTrajectory());
		vEnemies[i]->getTrajectory()->loadSteps(steps,true);
		vEnemies[i]->getTrajectory()->setNodeToLookAt(player.shipNode);
	}
}

std::vector<iceStep> iceLevel::getStepsFromResources(ConfigNode* p_sStepsNode)
{
	std::vector<iceStep> steps;

	int stepsNumber = p_sStepsNode->findChild("stepsNumber")->getValueI();
	steps.resize(stepsNumber);

	for(int i=0;i<stepsNumber;i++)
	{
		char stepName[10];
		sprintf(stepName,"step%d",i+1);
		ConfigNode* stepNode = p_sStepsNode->findChild(stepName);

		Ogre::Real posX = stepNode->findChild("position")->getValueF(0);
		Ogre::Real posY = stepNode->findChild("position")->getValueF(1);
		Ogre::Real posZ = stepNode->findChild("position")->getValueF(2);

		Ogre::Radian rollAngle = Ogre::Degree(stepNode->findChild("rollAngle")->getValueF());
		Ogre::Real time = stepNode->findChild("time")->getValueF();
		steps[i] = iceStep(Ogre::Vector3(posX,posY,posZ),rollAngle,time);
	}

	return steps;
}

const Ogre::String& iceLevel::getMusicName() const{
	return _musicName;
}
