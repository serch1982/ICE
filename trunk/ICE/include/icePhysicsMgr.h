#ifndef __ICE_PHYSICS_H__
#define __ICE_PHYSICS_H__

#include "OgreBulletDynamicsRigidBody.h"
#include "Ogre.h"
#include <deque>

class icePhysicsMgr{

	icePhysicsMgr( Ogre::SceneManager* p_SceneMgr, Ogre::Vector3 &gravityVector, Ogre::AxisAlignedBox &bounds );
	icePhysicsMgr();
	~icePhysicsMgr();

	// SetLevel
	void setLevel();
	// Reset Physics world
	void reset();

private:
	Ogre::SceneManager* mSceneMgr;
	OgreBulletDynamics::DynamicsWorld* mWorld;
	OgreBulletCollisions::DebugDrawer* mDebugDrawer;
	int mNumEntitiesInstanced;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
};

#endif