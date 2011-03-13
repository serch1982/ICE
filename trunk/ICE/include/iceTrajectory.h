#pragma once
#include <vector>
#include "iceTrajectoryStep.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreAnimation.h>

using namespace std;

class iceTrajectory
{
	public:
		iceTrajectory(void);
		~iceTrajectory(void);

	//attributes
	private:
		vector<iceTrajectoryStep> steps;
		Ogre::Real duration;
		Ogre::AnimationState* animState;

	//methods
	public:
		void addStep(iceTrajectoryStep step);
		void addTime(Ogre::Real time);
		void loadSteps(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node);
};

