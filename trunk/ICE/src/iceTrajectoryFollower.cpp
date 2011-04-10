#include "iceTrajectoryFollower.h"

iceTrajectoryFollower::iceTrajectoryFollower()
{
	mTrajectory = new iceTrajectory();
}

iceTrajectoryFollower::~iceTrajectoryFollower()
{
	delete mTrajectory;
}

bool iceTrajectoryFollower::initialize(Ogre::SceneManager* p_psSceneManager, Ogre::SceneNode* p_psNode)
{
	mSceneManager = p_psSceneManager;
	mNode = p_psNode;
	return true;
}

void iceTrajectoryFollower::finalize()
{

}

void iceTrajectoryFollower::update(Ogre::Real p_timeSinceLastFrame)
{
	mTrajectory->addTime(p_timeSinceLastFrame);
}

void iceTrajectoryFollower::setTrajectory(iceTrajectory* p_psTrajectory)
{
	delete mTrajectory;
	mTrajectory = p_psTrajectory;
	mTrajectory->init(mSceneManager,mNode);
}

iceTrajectory* iceTrajectoryFollower::getTrajectory(void)
{
	return mTrajectory;
}

Ogre::SceneNode* iceTrajectoryFollower::getNode(void)
{
	return mNode;
}
