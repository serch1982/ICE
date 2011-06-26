#include "Entities/iceCutSceneEntity.h"
#include "iceGame.h"

Ogre::NameGenerator iceCutSceneEntity::mNameGenerator("CutSceneEntity_");

iceCutSceneEntity::iceCutSceneEntity()
{
	mActivationTime = -1;
	mCurrentTime = 0;
}

iceCutSceneEntity::~iceCutSceneEntity()
{
}

bool iceCutSceneEntity::initialize(Ogre::String mMeshFile, Ogre::Vector3 p_Position, Ogre::Real p_ActivationTime, Ogre::SceneNode* pNodeToLookAt, iceTrajectory* pTrajectory)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mActivationTime = p_ActivationTime;
	mCurrentTime = 0;
	Ogre::String name = mNameGenerator.generate();
	mCutSceneEntityNode = sceneManager->getRootSceneNode()->createChildSceneNode(name);
	iceTrajectoryFollower::initialize(mCutSceneEntityNode);
	Ogre::Entity* mesh = sceneManager->createEntity(name + "_" + mMeshFile, mMeshFile);
	mNode->attachObject(mesh);
	mNode->setPosition(p_Position);

	if(pTrajectory)
	{
		setTrajectory(pTrajectory);
	}
	else
	{
		//Dummy Trajectory
		setTrajectory(new iceTrajectory());
	}
	if(pNodeToLookAt)
		mTrajectory->setNodeToLookAt(pNodeToLookAt);

	deactivate();

	return true;
}

void iceCutSceneEntity::finalize()
{
}

void iceCutSceneEntity::update(Ogre::Real p_TimeSinceLastFrame)
{
	if(mIsActive)
	{
		iceTrajectoryFollower::update(p_TimeSinceLastFrame);
		//mTrajectory->lookAt();
		if(mTrajectory->hasEnded())
			deactivate();
	}
	else
	{
			if(checkActivationTime(p_TimeSinceLastFrame))
			{//active
				activate();
			}
	}
}

Ogre::Real iceCutSceneEntity::getActivationTime(void)
{
	return mActivationTime;
}

void iceCutSceneEntity::setActivationTime(Ogre::Real p_ActivationTime)
{
	mActivationTime = p_ActivationTime;
}

bool iceCutSceneEntity::isActive(void)
{
	return mIsActive;
}

void iceCutSceneEntity::activate(void)
{
	mNode->setVisible(true);
	mIsActive = true;
}

void iceCutSceneEntity::deactivate(void)
{
	mNode->setVisible(false);
	mIsActive = false;
}

bool iceCutSceneEntity::checkActivationTime(Ogre::Real p_timeSinceLastFrame)
{
	if(mActivationTime < 0)
		return false;

	mCurrentTime += p_timeSinceLastFrame;
	if(mCurrentTime > mActivationTime)
	{
		mActivationTime = -1;
		return true;
	}
	else
	{
		return false;
	}
}
