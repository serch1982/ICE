#ifndef ICE_CUT_SCENE_ENTITY_H_
#define ICE_CUT_SCENE_ENTITY_H_

#include "Trajectory/iceTrajectoryFollower.h"
#include "Effects\iceAnimationMgr.h"
#include "Particle\iceParticleMgr.h"


class iceCutSceneEntity : public iceTrajectoryFollower
{
	public:
		iceCutSceneEntity();
		~iceCutSceneEntity();

		enum TYPE
		{
			SHIP = 0,
			MINI = 1,
			SMART = 2,
			KAMIKACE = 3,
			VOLCANO = 4,
			MAGMATON = 5,
			PIGSHEEP = 6
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

		void startAnimation(Ogre::String name);
		void stopAnimations();
		bool hasAnimationEnded(Ogre::String name);

	protected:
		static Ogre::NameGenerator mNameGenerator;

		TYPE mEntityType;

		bool mIsActive;

		Ogre::SceneNode* mCutSceneEntityNode;
		Ogre::Real mCurrentTime;
		Ogre::Real mActivationTime;

		iceAnimationMgrPtr iceAnimationPtr;
		iceParticlePtr partice1;
};

#endif