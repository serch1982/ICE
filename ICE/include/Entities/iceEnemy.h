#ifndef ICEENEMY_H_
#define ICEENEMY_H_

#include "Trajectory\iceTrajectoryFollower.h"
#include "iceRPG.h"
#include "icePlayer.h"
#include "icePhysicEntity.h"
#include "iceBullet.h"

class iceEnemy : public iceTrajectoryFollower, public iceRPG, public icePhysicEntity
{
	public:
		iceEnemy();
		~iceEnemy();

		//Enemy States
		enum ENEMYSTATE
		{
			STOPPED = 0, //En realidad esta siguiendo la trayectoria del player
			FOLLOWING_TRAJECTORY = 1,
			ATTACK = 2,
			DYING = 3,
			DEAD = 4,
			INACTIVE = 5
		};

		//Enemy Types
		enum ENEMYTYPE
		{
			MINIMAGMATON = 0,
			KAMIKAZE = 1,
			SMART = 2,
			VOLCANO = 3,
			MAGMATON = 4
		};

		void setState(ENEMYSTATE p_iState);
		int getState(void);
		ENEMYTYPE getType(void);

		void setPlayer(icePlayer* p_psPlayer);
			
		bool initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const ENEMYTYPE p_Type = MINIMAGMATON, const bool p_isAttachedToPlayer = false);
		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);
		void activate(void);
		bool checkActivationTime(Ogre::Real p_timeSinceLastFrame);
		bool isVisiblePlayerCam();
		bool isVisibleWideCam();
		float rangeAttack();
		bool isActive(void);

		// Animated Dead
		bool isAnimDyingEnded();
		void setAnimDyingEnded( Ogre::Real ticks );

		////iceRPG
		void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic);
		void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
		void showShieldDamage(unsigned int p_iDamage, bool p_bCritical);
		void showFail(void);
		void showLevelUp(unsigned int p_iLevel);
		
		vector<iceBullet*>* getAllBullets(void);
		Ogre::Vector3 getWorldPosition(void);
		virtual void showBoundingBox(void);
		virtual void hideBoundingBox(void);
	protected:
		static Ogre::NameGenerator mNameGenerator;

		ENEMYTYPE mType;
		ENEMYSTATE mState;
		icePlayer* mPlayer;
		Ogre::Real mCurrentTime;
		Ogre::Real mActivationTime;
		bool mShowingBoundingBox;

		Ogre::SceneNode* enemyNode, *enemyBulletNode;

		/*Bullet vectors depending on the weapon kind*/
		/*iceBullet mvMachinegunBullets[BULLET_VECTOR_SIZE];
		iceBullet mvShotgunBullets[BULLET_VECTOR_SIZE];
		iceBullet mvMisilLauncherBullets[BULLET_VECTOR_SIZE];*/
		vector<iceBullet*> mvMachinegunBullets;
		vector<iceBullet*> mvShotgunBullets;
		vector<iceBullet*> mvMisilLauncherBullets;
		void updateActiveBullets(Ogre::Real p_timeSinceLastFrame);

		// Animations
		//Ogre::AnimationState* mAttack01;
		//Ogre::AnimationState* mAttack02;
		//bool mbAnimAttack;


		Ogre::RaySceneQuery *mRaySceneQuery;

		Ogre::Real mAnimDyingTicks;
};

#endif