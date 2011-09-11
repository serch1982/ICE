#ifndef ICEPLAYER_H_
#define ICEPLAYER_H_

#include "Trajectory\iceTrajectoryFollower.h"
#include <vector>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "Entities\iceRPG.h"
#include <OgreLog.h>
#include "Utils\iceSdkTray.h"
#include "Stats\icePlayerStats.h"
#include "Particle\iceParticleMgr.h"
#include "Entities\icePhysicEntity.h"
#include "Entities\iceBulletMgr.h"
#include <boost/enable_shared_from_this.hpp>

class icePlayer : public iceTrajectoryFollower
                , public iceRPG
                , public icePhysicEntity
                , public Ogre::Singleton<icePlayer>
			    , public boost::enable_shared_from_this<icePlayer>
{
	public:
		icePlayer();
		~icePlayer();
        static icePlayer& getSingleton(void);
	    static icePlayer* getSingletonPtr(void);

		void initPlayer();

		void setCamera(Ogre::Camera* camera);
		Ogre::Camera* getCamera();
		void processMouseMoved(const int x, const int y, const int z);
		void update(Ogre::Real p_timeSinceLastFrame);

		void addXUserDeviation(int p_iXDeviation);
		void addYUserDeviation(int p_iYDeviation);
		Ogre::Vector3 getPosition(void);

		char* getCurrentWeaponName(void);

		void setIsShooting(bool);
		bool getIsShooting(void);
		int getCurrentWeapon();

		void scroll(Ogre::Real x, Ogre::Real y);

		//non RPG habilities
		void barrelLeft(void);
		void barrelRight(void);
		void sprint(void);
		void brake(void);

		//debug
		void setDebug(bool vari){ getGeometry()->getMovableObject()->setVisible(vari); virtualCam->getMovableObject()->setVisible(vari);}

		Ogre::SceneNode* getShipNode(void) { return shipNode; }
		Ogre::Vector3 getShipLastPosition(void){ return _lastPosition;}
		Ogre::Vector3 getShipPosition(void){ return shipNode->getPosition();}
		void setShipTranslate(Ogre::Vector3 pos){ shipNode->translate(pos);}

		Ogre::AxisAlignedBox getVitualCamBBox(void);

		//motion
		void setMovingUp(bool pMovingUp);
		void setMovingDown(bool pMovingDown);
		void setMovingLeft(bool pMovingLeft);
		void setMovingRight(bool pMovingRight);

		//Redefinidas para tener en cuenta los logros
		void setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel);

		bool isPositionBackToPlayer(Ogre::Vector3 pPosition);

		Ogre::Real getTimeMultiplier(void);
		void resetPositions(void);

		bool isInvulnerable(void);
		void setInvulnerable(bool invunerable = true);
	protected:

		bool mMovingUp;
		bool mMovingDown;
		bool mMovingLeft;
		bool mMovingRight;
		void updateShipPosition(Ogre::Real frameTime);
		void updateLookAt(Ogre::Real frameTime);
		void updateScroll(Ogre::Real frameTime);
		void changeWeapon(const int z);
		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(unsigned int p_iLevel);

		Ogre::Real mCameraWidth;
		Ogre::Real mCameraHeight;

		Ogre::Real mMaxScrollX; //Absolute value
		Ogre::Real mMaxScrollY; //Absolute value

		Ogre::SceneNode *cursorPlaneNode, *shipPlaneNode, *cameraPlaneNode, *cursorNode, *shipNode, *cameraNode, *scrollNode, *rollNode; 
		//virtual node for the wide camera boundingbox
		Ogre::SceneNode* snVirtualCam;
		iceGeometryPtr virtualCam;
		Ogre::Real shipMaxVelocity, _velocityX, _velocityY,_rolling;
		Ogre::Camera* playerCamera;
		Ogre::Log* mLog;
		Ogre::Vector3 _lastPosition;
		int mXUserDeviation;
		int mYUserDeviation;
		bool _isShooting;

		Ogre::Real mLeftBarrelTime; //Contadores, ha habilidad esta activa mientras el contador sea mayor que cero
		Ogre::Real mRightBarrelTime; //Contadores, ha habilidad esta activa mientras el contador sea mayor que cero
		Ogre::Real mBrakeTime; //Contadores, ha habilidad esta activa mientras el contador sea mayor que cero
		Ogre::Real mSprintTime; //Contadores, ha habilidad esta activa mientras el contador sea mayor que cero

		void updateLeftBarrel(Ogre::Real pTimeSinceLastEvent);
		void updateRightBarrel(Ogre::Real pTimeSinceLastEvent);
		void updateBarrelCommon(Ogre::Real* pTime, Ogre::Real pTimeSinceLastEvent, int pDirection);
		
		//particles
		iceParticlePtr mParticleTurboLeft;
		iceParticlePtr mParticleTurboRight;

		bool mInvulnerable;
		Ogre::Real mInvulnerableTime;
		Ogre::Real mBlinkTime;
		bool mBlinkVisible;
};

typedef boost::shared_ptr<icePlayer> icePlayerPtr;

#endif