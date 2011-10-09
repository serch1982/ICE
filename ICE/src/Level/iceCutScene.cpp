#include "Level/iceCutScene.h"
#include "Logic/iceLogicLua.h"
#include "iceGame.h"
#include "PostProcess\icePostProcessManager.h"

//Ogre::NameGenerator iceCutSceneEntity::mNameGenerator("CutSceneEntity_");

iceCutScene::iceCutScene()
{
	mActivationTime = -1;
	mIsPlaying = false;
	mStep = 0;
	mCurrentTime = 0;
}

iceCutScene::~iceCutScene()
{
	for(unsigned int i=0;i<mEntities.size();i++)
	{
		delete mEntities[i];
	}
}

bool iceCutScene::initialize(Ogre::String pInitFunctionStr,Ogre::String pUpdateFunctionStr, std::vector<iceTrajectory*>* pTrajectories)
{
	mInitFunctionStr = pInitFunctionStr;
	mUpdateFunctionStr = pUpdateFunctionStr;
	mTrajectories = pTrajectories;

	iceLogicLua::getInstance()->initCutScene(this);
	mCurrentTime = 0;
	cameraStarted = true;


	//initializeEntities();
	//initializeCameraEntity(2);

	return true;
}

void iceCutScene::filalize(void)
{
}

void iceCutScene::setActivationTime(Ogre::Real pActivationTime)
{
	mActivationTime = pActivationTime;
}

void iceCutScene::update(Ogre::Real p_TimeSinceLastFrame)
{
	if(!mIsPlaying)
		return;
	mCurrentTime += p_TimeSinceLastFrame;
	if(cameraStarted)
		mCameraEntity->update(p_TimeSinceLastFrame);
	for(unsigned int i=0;i<mEntities.size();i++)
	{
		mEntities[i]->update(p_TimeSinceLastFrame);
	}
	iceLogicLua::getInstance()->updateCutScene(this,p_TimeSinceLastFrame);
}
		
void iceCutScene::play(void)
{
	mIsPlaying = true;
	//prepare camera
	mLastCameraNode = iceGame::getCamera()->getParentSceneNode();
	iceGame::getCamera()->detachFromParent();
	mCameraEntity->getNode()->attachObject(iceGame::getCamera());
	//put overlay bands
	putBands();
}

void iceCutScene::rollback(void)
{

}

Ogre::Real iceCutScene::getActivationTime(void)
{
	return mActivationTime;
}

void iceCutScene::initializeEntities()
{
	//TODO scripted
	//addEntity(iceCutSceneEntity::SHIP,1);
	//entities.push_back(new iceCutSceneEntity());
	//entities[1]->initialize("airplane.mesh",Ogre::Vector3(0,0,0),0,NULL,trajectories[2]);
}

void iceCutScene::initializeCameraEntity(int pTrajectoryIndex)
{
	//TODO scripted
	mCameraEntity = new iceTrajectoryFollower();
	mCameraEntity->initialize(iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode());
	if(pTrajectoryIndex == -1)
	{
		mCameraEntity->setTrajectory(new iceTrajectory());
	}
	else
	{
		mCameraEntity->setTrajectory((*mTrajectories)[(unsigned int)pTrajectoryIndex]);
	}
	//setEntityLookAt(-1,0);
}
//Index == -1 => entity = camera
void iceCutScene::setEntityLookAt(int pEntityIndexLooking, int pEntityIndexLooked)
{
	Ogre::SceneNode* nodeToLookAt = NULL;
	if(pEntityIndexLooked == -1)
	{
		nodeToLookAt = mCameraEntity->getNode();
	}
	else if (pEntityIndexLooked >= 0)
	{
		nodeToLookAt = mEntities[(unsigned int)pEntityIndexLooked]->getNode();
	}

	if(pEntityIndexLooking == -1)
	{
		mCameraEntity->getTrajectory()->setNodeToLookAt(nodeToLookAt);
	}
	else if (pEntityIndexLooking >= 0)
	{
		mEntities[(unsigned int)pEntityIndexLooking]->getTrajectory()->setNodeToLookAt(nodeToLookAt);
	}
}

bool iceCutScene::hasEnded(void)
{
	return !mIsPlaying;
}

void iceCutScene::stop(void)
{
	hideToBeContinued();
	//remove overlay bands
	removeBands();
	//restore camera
	iceGame::getCamera()->detachFromParent();
	mLastCameraNode->attachObject(iceGame::getCamera());
	mActivationTime = -1;

	//hide actors
	for(unsigned int i=0;i<mEntities.size();i++)
	{
		mEntities[i]->deactivate();
	}
	mIsPlaying = false;
}

void iceCutScene::putBands(void)
{
	iceGame::getUI()->getHUD()->showFrame();
}

void iceCutScene::removeBands(void)
{
	iceGame::getUI()->getHUD()->hideFrame();
}

int iceCutScene::getStep()
{
	return mStep;
}

void iceCutScene::setStep(int pStep)
{
	mStep = pStep;
}

void iceCutScene::nextStep()
{
	mStep++;
}

Ogre::String iceCutScene::getInitFunctionStr()
{
	return mInitFunctionStr;
}

Ogre::String iceCutScene::getUpdateFunctionStr()
{
	return mUpdateFunctionStr;
}

//pEntityIndexToLookAt == -1 => look at camera;pEntityIndexToLookAt == -2 => look at nothing
void iceCutScene::addEntity(iceCutSceneEntity::TYPE pEntityType, int pTrajectoryIndex)
{
	iceCutSceneEntity* entity = new iceCutSceneEntity();
	if(pTrajectoryIndex == -1)
	{
		entity->initialize(pEntityType,Ogre::Vector3(0,0,0),0);
	}
	else
	{
		entity->initialize(pEntityType,Ogre::Vector3(0,0,0),0,(*mTrajectories)[(unsigned int)pTrajectoryIndex]);
	}
	mEntities.push_back(entity);
}

bool iceCutScene::isEntityTrajectoryEnded(int pEntityIndex)
{
	if(pEntityIndex == -1)
	{
		return mCameraEntity->getTrajectory()->hasEnded();
	}
	else
	{
		return mEntities[(unsigned int)pEntityIndex]->getTrajectory()->hasEnded();
	}
}

void iceCutScene::startEntityAnimation(int pEntityIndex, Ogre::String name)
{
	mEntities[(unsigned int)pEntityIndex]->startAnimation(name);
}

void iceCutScene::stopEntityAnimations(int pEntityIndex)
{
	mEntities[(unsigned int)pEntityIndex]->stopAnimations();
}

bool iceCutScene::hasEntityAnimationEnded(int pEntityIndex, Ogre::String name)
{
	return mEntities[(unsigned int)pEntityIndex]->hasAnimationEnded(name);
}

Ogre::Real iceCutScene::getCurrentTime()
{
	return mCurrentTime;
}

void iceCutScene::playSound(int i, float volume)
{
	iceSoundManager::getSingletonPtr()->PlaySound(i,Ogre::Vector3::ZERO,0,volume);
}

void iceCutScene::stopSound(int i)
{
	iceSoundManager::getSingletonPtr()->StopSound(&i);
}

void iceCutScene::stopAllSounds()
{
	iceSoundManager::getSingletonPtr()->StopAllSounds();
}

void iceCutScene::startEntity(int pEntityIndex)
{
	if(pEntityIndex == -1)
	{
		cameraStarted = true;
	}
	else
	{
		mEntities[(unsigned int)pEntityIndex]->start();
	}
}

void iceCutScene::stopEntity(int pEntityIndex)
{
	if(pEntityIndex == -1)
	{
		cameraStarted = false;
	}
	else
	{
		mEntities[(unsigned int)pEntityIndex]->stop();
	}
}

void iceCutScene::showBlur()
{
	icePostProcessManager::getSingleton().enableHardBlur();
}

void iceCutScene::hideBlur()
{
	icePostProcessManager::getSingleton().disableBlur();
}

void iceCutScene::showToBeContinued()
{
	iceGame::getUI()->getHUD()->showToBeContinued();
}

void iceCutScene::hideToBeContinued()
{
	iceGame::getUI()->getHUD()->hideToBeContinued();
}