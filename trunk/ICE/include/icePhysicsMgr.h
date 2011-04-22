#ifndef __ICE_PHYSICS_H__
#define __ICE_PHYSICS_H__

#include "OgreBulletDynamicsRigidBody.h"
#include "OgreAxisAlignedBox.h"
#include "OgreSceneNode.h"
#include "OgreLog.h"
#include "OgreLogManager.h"
#include "Ogre.h"

#include <deque>
#include "icePlayer.h"

#define BIT(x) (1<<(x))

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
	void addPlayer( icePlayer& pPlayer );
	// Reset Physics world
	void reset();
	// Update
	void update( Ogre::Real time );
	void setShowDebug( bool p_bShow );

private:
	enum collisiontypes {
		COL_NOTHING = 0, //<Collide with nothing
		COL_SHIP = BIT(0), //<Collide with ships
		COL_WALL = BIT(1), //<Collide with walls
	//	COL_POWERUP = BIT(2) //<Collide with powerups
	};

	int mShipCollidesWith;
	int mWallCollidesWith;
	//int powerupCollidesWith = COL_SHIP | COL_WALL;


	// SceneManager of OGRE
	Ogre::SceneManager* mSceneMgr;
	// Physical World
	OgreBulletDynamics::DynamicsWorld* mWorld;
	// Debug
	OgreBulletCollisions::DebugDrawer* mDebugDrawer;

	// Shape Manager - Maybe could be deleted
	int mNumEntitiesInstanced;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

	// Game Log
	Ogre::Log* mGameLog;

	// Set levels
	void setLevel1();
	void setLevel2();
	void setLevel3();
};

#endif