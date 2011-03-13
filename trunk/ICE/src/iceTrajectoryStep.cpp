#include "iceTrajectoryStep.h"


iceTrajectoryStep::iceTrajectoryStep(void)
{
	position = 0;
	rollAngle = 0;
	time = 0;
}

iceTrajectoryStep::iceTrajectoryStep(Ogre::Vector3 pos,Ogre::Radian rollA,Ogre::Real tim)
{
	position = pos;
	rollAngle = rollA;
	time = tim;
}


iceTrajectoryStep::~iceTrajectoryStep(void)
{
}
