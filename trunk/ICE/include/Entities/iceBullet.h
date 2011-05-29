#ifndef ICEBULLET_H_
#define ICEBULLET_H_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>
#include <OgrePrerequisites.h>
#include <OgreBillboard.h>
#include <OgreBillboardSet.h>
#include <OgreParticleSystemManager.h>

#include "icePhisicEntity.h"

#define MAX_TIME_ACTIVE	  5	// Maximum time that the bullet can fly without having been crashed

class iceBullet : public icePhisicEntity
{
public:
iceBullet(void);
	
	virtual ~iceBullet(void);

	void CreateEntities(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* bulletNode, int p_iWeapon, int p_iBulletNumber);
	bool Set(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* shipNode,Ogre::Real p_iDamage, bool p_bCritic, int p_iShotSide);	/* Returns true when a bullet is set to active status*/
	
	void Update(Ogre::Real timeSinceLastFrame);
	bool isActive(void);
	Ogre::Vector3 getWorldPosition(void);

	unsigned int getDamage(void);
	bool isCritic(void);
	void deactivate(void);

protected:
	bool mbActive;						// Bullet has been shooted and is now into the scene.
	
	int miWeapon;						// Bullet's weapon type
	unsigned int miDamage;				// Bullet's damage
	bool mbCritic;						// It makes bullet be letal	
	int miSpeed;						// Bullet's speed. It will depend on the kind of weapon
	Ogre::Real miCountDown;				// Bullet's live time

	Ogre::SceneNode* msbulletNode;		// Bullet's sceneNode object
	
	
	Ogre::Vector3 mvPosition;			// Bullet's world position vector
	Ogre::Quaternion msOrientation;		// Bullet's world direction quaternion
};
#endif