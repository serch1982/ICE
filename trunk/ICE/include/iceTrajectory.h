#pragma once
#include <vector>
#include "iceStep.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreAnimation.h>
#include <OgreLog.h>
#include "iceChivatos.h"

using namespace std;

#define LOCOMOTIVE_ADVANCE 1.0f //Avance de la locomotora en segundos

class iceTrajectory
{
	public:
		iceTrajectory(void);
		~iceTrajectory(void);

	//attributes
	private:
		Ogre::SimpleSpline mTrack;
		vector<iceStep> mSteps;
		Ogre::SceneNode* mNode;
		Ogre::SceneNode* mLocomotiveNode;
		Ogre::Real mDuration;
		Ogre::Real mCurrentTime;
		Ogre::SceneManager* mSceneManager;

		Ogre::Log* mLog;

	//methods
	public:
		void loadSteps(vector<iceStep> p_vSteps);
		void addTime(Ogre::Real p_fTime);
		void init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode);

	protected:
		unsigned int getCurrentStepIndexByTime(Ogre::Real p_fTime);
		Ogre::Real getInterpolationByTime(Ogre::Real p_fTime);
		Ogre::Radian getInterpolatedRollByTime(Ogre::Real p_fTime);
};

