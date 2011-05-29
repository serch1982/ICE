#pragma once
#include <OgreVector3.h>
class iceStep
{
public:
	iceStep(void);
	iceStep(Ogre::Vector3 p_vPosition,Ogre::Radian p_rRollAngle,Ogre::Real p_rTime);
	~iceStep(void);

	Ogre::Vector3 getPosition(void);
	Ogre::Radian getRollAngle(void);
	Ogre::Real getTime(void);

	bool operator<(iceStep rs);

protected:
	Ogre::Vector3 mPosition;
	Ogre::Radian mRollAngle;
	Ogre::Real mTime;
};

