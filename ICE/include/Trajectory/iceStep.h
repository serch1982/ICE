#pragma once
#include <OgreVector3.h>
class iceStep
{
public:
	iceStep(void);
	iceStep(Ogre::Vector3 p_vPosition,Ogre::Quaternion pRotation,Ogre::Vector3 pScale,Ogre::Real p_rTime);
	~iceStep(void);

	Ogre::Vector3 getPosition(void);
	Ogre::Quaternion getRotation(void);
	Ogre::Vector3 getScale(void);
	Ogre::Real getTime(void);

	bool operator<(iceStep rs);

protected:
	Ogre::Vector3 mPosition;
	Ogre::Quaternion mRotation;
	Ogre::Vector3 mScale;
	Ogre::Real mTime;
};

