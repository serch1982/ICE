#include "Trajectory\iceStep.h"


iceStep::iceStep(void)
{
	mPosition = 0;
	mRollAngle = 0;
	mTime = 0;
}

iceStep::iceStep(Ogre::Vector3 p_vPosition,Ogre::Radian p_rRollAngle,Ogre::Real p_rTime)
{
	mPosition = p_vPosition;
	mRollAngle = p_rRollAngle;
	mTime = p_rTime;
}
iceStep::~iceStep(void)
{
}

Ogre::Vector3 iceStep::getPosition(void){ return mPosition; }
Ogre::Radian iceStep::getRollAngle(void){ return mRollAngle; }
Ogre::Real iceStep::getTime(void){ return mTime; }
