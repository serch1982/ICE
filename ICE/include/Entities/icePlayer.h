#ifndef ICEPLAYER_H_
#define ICEPLAYER_H_

#include "Trajectory\iceTrajectoryFollower.h"
#include <vector>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "iceRPG.h"
#include "iceBullet.h"
#include <OgreLog.h>
#include "Utils\iceSdkTray.h"
#include "Stats\icePlayerStats.h"


class icePlayer : public iceTrajectoryFollower, public iceRPG
{
	public:
		icePlayer();
		~icePlayer();
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
	protected:

		void updateShipPosition(Ogre::Real frameTime);
		void updateActiveBullets(Ogre::Real p_timeSinceLastFrame);
		void changeWeapon(const int z);
		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(unsigned int p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(unsigned int p_iLevel);

		
	public:
		bool _isShooting;

		//Redefinidas para tener en cuenta los logros
		void setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel);

		Ogre::SceneNode *cursorPlaneNode, *shipPlaneNode, *cameraPlaneNode, *cursorNode, *shipNode, *cameraNode; 
		Ogre::Real shipMaxVelocity;
		Ogre::Camera* playerCamera;
		Ogre::Log* mLog;

		int mXUserDeviation;
		int mYUserDeviation;
		
		//Pau * BULLETS------------------------------------------------//
		
		vector<iceBullet*>* getAllBullets(void);

		/*Bullet's Father node: son of the root's node and placed in absolute 0,0,0 coordinates.
		  Every bullet is gonna be son of the mainBulletNode*/
		Ogre::SceneNode *mainBulletNode;

		/*Bullet vectors depending on the weapon kind*/
		vector<iceBullet*> mvMachinegunBullets;
		vector<iceBullet*> mvShotgunBullets;
		vector<iceBullet*> mvMisilLauncherBullets;
		//--------------------------------------------------------------//
};

#endif