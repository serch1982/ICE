#ifndef __ICE_TRAJECTORYFOLLOWER_H__
#define __ICE_TRAJECTORYFOLLOWER_H__

#include "iceTrajectory.h"

using namespace std;

class iceTrajectoryFollower
{
	public:
		iceTrajectoryFollower();
		~iceTrajectoryFollower();

		virtual bool initialize(Ogre::SceneManager* p_psSceneManager, Ogre::SceneNode* p_psNode);
		void finalize();

		virtual void update(Ogre::Real p_timeSinceLastFrame);
		void setTrajectory(iceTrajectory* p_psTrajectory);
		iceTrajectory* getTrajectory(void);

		Ogre::SceneNode* getNode(void);

	protected:
		Ogre::SceneManager* mSceneManager;
		iceTrajectory* mTrajectory;
		Ogre::SceneNode* mNode;
};

#endif