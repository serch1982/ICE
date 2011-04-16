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
			INACTIVE = 4
		};

		void setState(ENEMYSTATE p_iState);
		int getState(void);

		void setPlayer(icePlayer* p_psPlayer);
			
		bool initialize(Ogre::SceneManager* p_psSceneManager, icePlayer* p_psPlayer);
		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);
		void activate(void);

		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(unsigned int p_iLevel);
	
	protected:
		static Ogre::NameGenerator mNameGenerator;

		ENEMYSTATE mState;
		icePlayer* mPlayer;
};

#endif