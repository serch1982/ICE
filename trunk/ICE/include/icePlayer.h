#ifndef __ICE_PLAYER_H__
#define __ICE_PLAYER_H__

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

#define MAX_BULLETS_PER_WEAPON	  10	// Maximum number of bullets active at the same time per weapon
using namespace std;

class icePlayer : public iceRPG
{
	public:
		icePlayer();
		~icePlayer();
		bool initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node);
		void setCamera(Ogre::Camera* camera);
		Ogre::Camera* getCamera();
		void processMouseMoved(const OIS::MouseEvent &arg);
		void updateShipPosition(Ogre::Real frameTime);

		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);

	protected:

		//iceRPG
		virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic); 
		virtual void showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical);
		virtual void showFail(void);
		virtual void showLevelUp(void);
public:
		Ogre::SceneNode *playerNode, *cursorPlaneNode, *shipPlaneNode, *cameraPlaneNode, *cursorNode, *shipNode, *cameraNode, *mainBulletNode;
		Ogre::Real shipMaxVelocity;
		Ogre::Camera* playerCamera;
		vector<iceBullet> mvBullets;
};

#endif