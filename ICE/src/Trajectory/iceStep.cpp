#include "Trajectory\iceStep.h"


iceStep::iceStep(void)
{
	mPosition = Ogre::Vector3::ZERO;
	mRotation = Ogre::Quaternion::IDENTITY;
	mScale = Ogre::Vector3::UNIT_SCALE;
	mTime = 0;
}

iceStep::iceStep(Ogre::Vector3 p_vPosition,Ogre::Quaternion pRotation,Ogre::Vector3 pScale,Ogre::Real p_rTime)
{
	mPosition = p_vPosition;
	mRotation = pRotation;
	mScale = pScale;
	mTime = p_rTime;
}
iceStep::~iceStep(void)
{
}

bool iceStep::operator<(iceStep rs) { return mTime < rs.getTime(); }

Ogre::Vector3 iceStep::getPosition(void){ return mPosition; }
Ogre::Quaternion iceStep::getRotation(void){ return mRotation; }
Ogre::Vector3 iceStep::getScale(void){ return mScale; }
Ogre::Real iceStep::getTime(void){ return mTime; }
