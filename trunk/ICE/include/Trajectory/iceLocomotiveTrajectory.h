#pragma once
#include "iceTrajectory.h"

using namespace std;

#define LOCOMOTIVE_ADVANCE 1.0f //Avance de la locomotora en segundos

class iceLocomotiveTrajectory : public iceTrajectory
{
	public:
		iceLocomotiveTrajectory(void);
		~iceLocomotiveTrajectory(void);

	//attributes
	protected:
		Ogre::SceneNode* mLocomotiveNode;



	//methods
	public:
		void init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode);
	protected:
		void addTime(Ogre::Real p_fTime);
		Ogre::Radian getInterpolatedRollByTime(Ogre::Real p_fTime);
};

