#pragma once
#include <OgreVector3.h>
class iceTrajectoryStep
{
public:
	iceTrajectoryStep(void);
	iceTrajectoryStep(Ogre::Vector3 pos,Ogre::Radian rollA,Ogre::Real tim);
	~iceTrajectoryStep(void);

	Ogre::Vector3 position;
	Ogre::Radian rollAngle;
	Ogre::Real time;
};

