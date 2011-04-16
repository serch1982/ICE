#include "icePhysicsMgr.h"
#include "OgreBulletCollisionsShape.h"


icePhysicsMgr::icePhysicsMgr( Ogre::SceneManager* p_SceneMgr,
	Ogre::Vector3 &gravityVector, Ogre::AxisAlignedBox &bounds  ){

	mNumEntitiesInstanced = 0; // how many shapes are created
    mSceneMgr = p_SceneMgr;    // getting the scene manager
    mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector); // Start Bullet
 
	// add Debug info display tool
    mDebugDrawer = new OgreBulletCollisions::DebugDrawer();
	// we want to see the Bullet containers
    mDebugDrawer->setDrawWireframe(true);

	// enable it if you want to see the Bullet containers
    mWorld->setDebugDrawer(mDebugDrawer);
    mWorld->setShowDebugShapes(true);      
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject(static_cast <Ogre::SimpleRenderable *> (mDebugDrawer));
}

icePhysicsMgr::icePhysicsMgr(){

	mNumEntitiesInstanced = 0; // how many shapes are created
    mSceneMgr = NULL;    // getting the scene manager

	Ogre::AxisAlignedBox aaB( Ogre::Vector3 (-10000, -10000, -10000), //aligned box for Bullet
							  Ogre::Vector3 (10000,  10000,  10000));
    mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, aaB, Ogre::Vector3(0,-9.81,-0) ); // Start Bullet
 
	// add Debug info display tool
    mDebugDrawer = new OgreBulletCollisions::DebugDrawer();
	// we want to see the Bullet containers
    mDebugDrawer->setDrawWireframe(true);

	// enable it if you want to see the Bullet containers
    mWorld->setDebugDrawer(mDebugDrawer);
    mWorld->setShowDebugShapes(true);

//	if( mSceneMgr != NULL ){
//		Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
//		node->attachObject(static_cast <Ogre::SimpleRenderable *> (mDebugDrawer));
//	}

}

icePhysicsMgr::~icePhysicsMgr(){

	// OgreBullet physic delete - RigidBodies
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {   
       delete *itBody;
       ++itBody;
    }   
    // OgreBullet physic delete - Shapes
    std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
    while (mShapes.end() != itShape)
    {   
       delete *itShape;
       ++itShape;
    }

	// clear deque
    mBodies.clear();
    mShapes.clear();

	// delete debug drawer
    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    
	//delete world
	delete mWorld;
}

void icePhysicsMgr::setLevel(){

}

void icePhysicsMgr::reset(){
	
	// OgreBullet physic delete - RigidBodies
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {   
       delete *itBody;
       ++itBody;
    }   
    // OgreBullet physic delete - Shapes
    std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
    while (mShapes.end() != itShape)
    {   
       delete *itShape;
       ++itShape;
    }

	// clear deque
    mBodies.clear();
    mShapes.clear();

}