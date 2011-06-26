#include "Level/iceCutScene.h"
#include "iceGame.h"

//Ogre::NameGenerator iceCutSceneEntity::mNameGenerator("CutSceneEntity_");

iceCutScene::iceCutScene()
{
	mActivationTime = -1;
	mIsPlaying = false;
}

iceCutScene::~iceCutScene()
{
	for(unsigned int i=0;i<entities.size();i++)
	{
		delete entities[i];
	}
}

bool iceCutScene::initialize(Ogre::String luaScript, std::vector<iceTrajectory*>& trajectories, Ogre::Real pActivationTime)
{
	initializeEntities(trajectories);
	initializeCameraEntity(trajectories);
	mActivationTime = pActivationTime;

	return true;
}

void iceCutScene::filalize(void)
{
}

void iceCutScene::update(Ogre::Real p_TimeSinceLastFrame)
{
	if(!mIsPlaying)
		return;
	cameraEntity->update(p_TimeSinceLastFrame);
	for(unsigned int i=0;i<entities.size();i++)
	{
		entities[i]->update(p_TimeSinceLastFrame);
	}
}
		
void iceCutScene::play(void)
{
	mIsPlaying = true;
	//prepare camera
	lastCameraNode = iceGame::getCamera()->getParentSceneNode();
	iceGame::getCamera()->detachFromParent();
	cameraEntity->getNode()->attachObject(iceGame::getCamera());
	//put overlay bands
	putBands();
}

void iceCutScene::rollback(void)
{
	//remove overlay bands
	removeBands();
	//restore camera
	iceGame::getCamera()->detachFromParent();
	lastCameraNode->attachObject(iceGame::getCamera());
	mActivationTime = -1;
}

Ogre::Real iceCutScene::getActivationTime(void)
{
	return mActivationTime;
}

void iceCutScene::initializeEntities(std::vector<iceTrajectory*>& trajectories)
{
	//TODO scripted
	entities.push_back(new iceCutSceneEntity());
	entities[0]->initialize("airplane.mesh",Ogre::Vector3(0,0,0),0,NULL,trajectories[1]);
	//entities.push_back(new iceCutSceneEntity());
	//entities[1]->initialize("airplane.mesh",Ogre::Vector3(0,0,0),0,NULL,trajectories[2]);
}

void iceCutScene::initializeCameraEntity(std::vector<iceTrajectory*>& trajectories)
{
	//TODO scripted
	cameraEntity = new iceTrajectoryFollower();
	cameraEntity->initialize(iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode());
	cameraEntity->setTrajectory(trajectories[2]);
	Ogre::SceneNode* no = entities[0]->getNode();
	cameraEntity->getTrajectory()->setNodeToLookAt(entities[0]->getNode());
}

bool iceCutScene::hasEnded(void)
{
	return cameraEntity->getTrajectory()->hasEnded();
}

void iceCutScene::putBands(void)
{
}

void iceCutScene::removeBands(void)
{
}