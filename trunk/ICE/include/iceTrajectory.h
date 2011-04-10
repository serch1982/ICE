#pragma once
#include <vector>
#include "iceStep.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreLog.h>
#include "iceDebugScreen.h"

class iceTrajectory
{
	public:
		iceTrajectory(void);
		~iceTrajectory(void);

	//attributes
	protected:
		Ogre::SimpleSpline mTrack;
		std::vector<iceStep> mSteps;
		Ogre::SceneNode* mNode;
		Ogre::SceneNode* mNodeToLookAt;
		Ogre::Real mDuration;
		Ogre::Real mCurrentTime;
		Ogre::SceneManager* mSceneManager;
		bool mThereIsANodeToLookAt;
		bool mLoop;

		Ogre::Log* mLog;

	//methods
	public:
		void loadSteps(std::vector<iceStep> p_vSteps, const bool p_bIsLoop = false);
		virtual void addTime(Ogre::Real p_fTime);
		virtual void init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode);
		void setNodeToLookAt(Ogre::SceneNode* p_psNodeToLookAt);
		void reset(void);
		void setLoop(bool p_bLoop);
		bool hasEnded(void);

	protected:
		unsigned int getCurrentStepIndexByTime(Ogre::Real p_fTime);
		Ogre::Real getInterpolationByTime(Ogre::Real p_fTime);
		void lookAt(void);
};

