#ifndef ICETRAJECTORYFOLLOWER_H_
#define ICETRAJECTORYFOLLOWER_H_

#include "iceTrajectory.h"

using namespace std;

class iceTrajectoryFollower
{
	public:
		iceTrajectoryFollower();
		~iceTrajectoryFollower();

		virtual bool initialize(Ogre::SceneNode* p_psNode);
		void finalize();

		virtual void update(Ogre::Real p_timeSinceLastFrame);
		void setTrajectory(iceTrajectory* p_psTrajectory);
		iceTrajectory* getTrajectory(void);

		Ogre::SceneNode* getNode(void);

	protected:
		iceTrajectory* mTrajectory;
		Ogre::SceneNode* mNode;
};

#endif