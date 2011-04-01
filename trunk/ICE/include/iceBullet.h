#ifndef ___ICEBULLET_H__
#define ___ICEBULLET_H__

#include <OgreFrameListener.h>
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>
#include <OgrePrerequisites.h>

#define SPEED	100				// Bullet's speed
#define MAX_TIME_ACTIVE	  10	// Maximum time that the bullet can fly without having been crashed

class iceBullet
{
public:
	iceBullet(void);
	virtual ~iceBullet(void);

	void set(Ogre::SceneNode* shipNode, Ogre::SceneNode* bulletNode,int p_iWeapon, Ogre::Real p_iDamage, bool p_bCritic);
	void update( Ogre::SceneNode* bulletNode, const Ogre::FrameEvent& evt);
		
protected:
	bool mbActive;						// Bullet has been shooted and is now into the scene.
	int miWeapon;						// Bullet's weapon type
	Ogre::Real miDamage;				// Bullet's damage
	bool mbCritic;						// It makes bullet be letal

	Ogre::Real miCountDown;				// Bullet's live time
	
	Ogre::Vector3 mvPosition;			// Bullet's world position vector
	Ogre::Quaternion msOrientation;		// Bullet's world direction quaternion
};
#endif