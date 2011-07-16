#ifndef ICEENEMY_H_
#define ICEENEMY_H_

#include "Trajectory\iceTrajectoryFollower.h"
#include "Entities\iceRPG.h"
#include "Entities\icePlayer.h"
#include "Entities\icePhysicEntity.h"
#include "Entities\iceBulletMgr.h"
#include "Effects\iceBillboard.h"
#include "Particle\iceParticleMgr.h"

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
		//set billboard
		void setBillboard(iceBillboard* billboard);
		//set particle pointer
		void setIceParticle(iceParticlePtr particlePtr);

		Ogre::SceneNode* getEnemySceneNode(void);

		// getter and setter for state
		virtual void setState(ENEMYSTATE p_iState);
		int getState(void);

		// Activation in the world
		void activate(void);
		void desactivate(void);
		// Waiting for activation
		bool checkActivationTime(Ogre::Real p_timeSinceLastFrame);
		// Is the enemy visible to the player
		bool isVisiblePlayerCam();
		// Is the enemy in the tube but not visible to the player?
		bool isVisibleWideCam();
		
		float rangeAttack();
		// Has been activated?
		bool isActive(void);

		// virtual functions for descendants
		virtual bool initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
		virtual void finalize();
		virtual void update(Ogre::Real p_timeSinceLastFrame);
		virtual std::string getFunctionStr();

		// Animated Dead
		bool isAnimDyingEnded();
		void setAnimDyingEnded( Ogre::Real ticks );

		////iceRPG
		// enemy shot
		void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic);
		// Showing information to user
		virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
		void showShieldDamage(unsigned int p_iDamage, bool p_bCritical);
		void showFail(void);
		void showLevelUp(unsigned int p_iLevel);
		
		//Getter for the position
		Ogre::Vector3 getWorldPosition(void);

		//debug
		void setDebug(bool vari){ getGeometry()->getMovableObject()->setVisible(vari); }
	protected:
		// Generator for obtain a name for this instance
		static Ogre::NameGenerator mNameGenerator;
		
		//Attributes
		ENEMYSTATE mState;
		//icePlayer* mPlayer;
		Ogre::Real mCurrentTime;
		Ogre::Real mActivationTime;
		bool mShowingBoundingBox;

		//Nodes, bullet necessary?
		Ogre::SceneNode* enemyNode, *enemyBulletNode;

		// Animations
		//Ogre::AnimationState* mAttack01;
		//Ogre::AnimationState* mAttack02;
		//bool mbAnimAttack;
		
		Ogre::RaySceneQuery *mRaySceneQuery;

		// Controlling ticks for dying animation
		Ogre::Real mAnimDyingTicks;

		//effect billboard
		iceBillboard* mBillboard;	

		// DEBUG
		Ogre::Log* mLog;
		
};

#endif