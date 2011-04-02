#ifndef ___ICEBULLET_H__
#define ___ICEBULLET_H__

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>
#include <OgrePrerequisites.h>


#define MAX_TIME_ACTIVE	  10	// Maximum time that the bullet can fly without having been crashed

class iceBullet
{
public:
	iceBullet(void);
	virtual ~iceBullet(void);

	void Set(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* shipNode, Ogre::SceneNode* bulletNode,
			 int p_iWeapon, Ogre::Real p_iDamage, bool p_bCritic);
	
	void Update(Ogre::Real timeSinceLastFrame);

protected:
	bool mbActive;						// Bullet has been shooted and is now into the scene.
	
	int miWeapon;						// Bullet's weapon type
	Ogre::Real miDamage;				// Bullet's damage
	bool mbCritic;						// It makes bullet be letal	
	int miSpeed;						// Bullet's speed. It will depend on the kind of weapon
	Ogre::Real miCountDown;				// Bullet's live time

	Ogre::SceneNode* msbulletNode;		// Bullet's sceneNode object
	
	
	Ogre::Vector3 mvPosition;			// Bullet's world position vector
	Ogre::Quaternion msOrientation;		// Bullet's world direction quaternion
};
#endif