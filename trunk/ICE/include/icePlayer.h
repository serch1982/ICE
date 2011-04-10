#ifndef __ICE_PLAYER_H__
#define __ICE_PLAYER_H__

#include "iceTrajectoryFollower.h"
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
#include "iceDebugScreen.h"
#include <OgreLog.h>


using namespace std;

class icePlayer : public iceTrajectoryFollower, public iceRPG
{
	public:
		icePlayer();
		~icePlayer();
		bool initialize(Ogre::SceneManager* p_psSceneManager, Ogre::SceneNode* p_psNode);
		void setCamera(Ogre::Camera* camera);
		Ogre::Camera* getCamera();
		void processMouseMoved(const OIS::MouseEvent &arg);
		void update(Ogre::Real p_timeSinceLastFrame);

		void finalize();

	protected:

		void updateShipPosition(Ogre::Real frameTime);
		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(unsigned int p_iLevel);

		
	public:
		//Redefinidas para tener en cuenta los logros
		void setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel);

		Ogre::SceneNode *cursorPlaneNode, *shipPlaneNode, *cameraPlaneNode, *cursorNode, *shipNode, *cameraNode; 
		Ogre::Real shipMaxVelocity;
		Ogre::Camera* playerCamera;
		Ogre::Log* mLog;
		
		//Pau * BULLETS------------------------------------------------//
		
		/*Bullet's Father node: son of the root's node and placed in absolute 0,0,0 coordinates.
		  Every bullet is gonna be son of the mainBulletNode*/
		Ogre::SceneNode *mainBulletNode;

		/*Bullet vectors depending on the weapon kind*/
		iceBullet mvMachinegunBullets[MACHINEGUN_RELOAD];
		iceBullet mvShotgunBullets[SHOTGUN_RELOAD];
		iceBullet mvMisilLauncherBullets[MISILE_LAUNCHER_RELOAD];
		//--------------------------------------------------------------//
};

#endif