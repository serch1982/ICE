#include "Trajectory\iceTrajectoryFollower.h"
#include "iceGame.h"

iceTrajectoryFollower::iceTrajectoryFollower()
{
	mTrajectory = new iceTrajectory();
}

iceTrajectoryFollower::~iceTrajectoryFollower()
{
	delete mTrajectory;
}

bool iceTrajectoryFollower::initialize(Ogre::SceneNode* p_psNode)
{
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
	//if(mTrajectory) delete mTrajectory;
	mTrajectory = p_psTrajectory;
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mTrajectory->init(sceneManager,mNode);
}

iceTrajectory* iceTrajectoryFollower::getTrajectory(void)
{
	return mTrajectory;
}

Ogre::SceneNode* iceTrajectoryFollower::getNode(void)
{
	return mNode;
}
