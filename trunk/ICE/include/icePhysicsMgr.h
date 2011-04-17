#ifndef __ICE_PHYSICS_H__
#define __ICE_PHYSICS_H__

#include "OgreBulletDynamicsRigidBody.h"
#include "Ogre.h"
#include <deque>
#include "OgreAxisAlignedBox.h"
#include "OgreSceneNode.h"
#include "icePlayer.h"

class icePhysicsMgr{

public:
	icePhysicsMgr();
	~icePhysicsMgr();

	// init the world
	void init( Ogre::SceneManager* p_SceneMgr, 
		Ogre::Vector3 &gravityVector = Ogre::Vector3( 0,-9.81,0), 
		Ogre::AxisAlignedBox &bounds = Ogre::AxisAlignedBox(
								Ogre::Vector3 (-10000, -10000, -10000),
								Ogre::Vector3 (10000,  10000,  10000)) );
	// SetLevel
	void setLevel( int p_iCurrentPhase );
	// Add player to physics world
	void icePhysicsMgr::addPlayer( icePlayer& pPlayer );
	// Reset Physics world
	void reset();
	// Update
	void update( Ogre::Real time );

private:
	Ogre::SceneManager* mSceneMgr;
	OgreBulletDynamics::DynamicsWorld* mWorld;
	OgreBulletCollisions::DebugDrawer* mDebugDrawer;
	int mNumEntitiesInstanced;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
};

#endif