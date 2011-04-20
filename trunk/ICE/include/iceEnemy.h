#ifndef __ICE_ENEMY_H__
#define __ICE_ENEMY_H__

#include "iceTrajectoryFollower.h"
#include "iceRPG.h"
#include "icePlayer.h"

class iceEnemy : public iceTrajectoryFollower, public iceRPG
{
	public:
		iceEnemy();
		~iceEnemy();

		//Enemy States
		enum ENEMYSTATE
		{
			STOPPED = 0, //En realidad esta siguiendo la trayectoria del player
			FOLLOWING_TRAJECTORY = 1,
			GOING_TO_PLAYER = 2,
			DEADING = 3,
			DEAD = 4,
			INACTIVE = 5
		};

		//Enemy Types
		enum ENEMYTYPE
		{
			MINIMAGMATON = 0,
			KAMIKAZE = 1,
			INTELLIGENT = 2,
			VOLCANO = 3,
			MAGMATON = 4
		};

		void setState(ENEMYSTATE p_iState);
		int getState(void);

		void setPlayer(icePlayer* p_psPlayer);
			
		bool initialize(Ogre::SceneManager* p_psSceneManager, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const ENEMYTYPE p_Type = MINIMAGMATON, const bool p_isAttachedToPlayer = false);
		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);
		void activate(void);
		bool checkActivationTime(Ogre::Real p_timeSinceLastFrame);

		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(unsigned int p_iLevel);
	
	protected:
		static Ogre::NameGenerator mNameGenerator;

		ENEMYTYPE mType;
		ENEMYSTATE mState;
		icePlayer* mPlayer;
		Ogre::Real mCurrentTime;
		Ogre::Real mActivationTime;
};

#endif