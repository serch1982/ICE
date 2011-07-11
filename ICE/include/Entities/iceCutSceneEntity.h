#ifndef ICE_CUT_SCENE_ENTITY_H_
#define ICE_CUT_SCENE_ENTITY_H_

#include "Trajectory/iceTrajectoryFollower.h"


class iceCutSceneEntity : public iceTrajectoryFollower
{
	public:
		iceCutSceneEntity();
		~iceCutSceneEntity();

		enum TYPE
		{
			SHIP = 0,
			MINI = 1
		};

		bool initialize(TYPE pEntityType, Ogre::Vector3 p_Position, Ogre::Real p_ActivationTime, iceTrajectory* pTrajectory = NULL);
		void finalize();

		void update(Ogre::Real p_TimeSinceLastFrame);

		Ogre::Real getActivationTime(void);
		void setActivationTime(Ogre::Real p_ActivationTime);

		bool isActive(void);
		void activate(void);
		void deactivate(void);

		bool checkActivationTime(Ogre::Real p_timeSinceLastFrame);

	protected:
		static Ogre::NameGenerator mNameGenerator;

		bool mIsActive;

		Ogre::SceneNode* mCutSceneEntityNode;
		Ogre::Real mCurrentTime;
		Ogre::Real mActivationTime;
};

#endif