#include "icePhysicsMgr.h"
#include "OgreBulletCollisionsShape.h"
#include <Shapes\OgreBulletCollisionsBoxShape.h>


icePhysicsMgr::icePhysicsMgr(){
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

void icePhysicsMgr::init( Ogre::SceneManager* p_SceneMgr,
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

void icePhysicsMgr::setLevel( int p_iCurrentPhase ){

	//TODO: 
	//Find level root node

	//Iterate the tree
		//Create Collision Shape
		//Add to the world
}

//TODO: Add player to physics world
void icePhysicsMgr::addPlayer( icePlayer& pPlayer ){
	
	//Get the player mesh node
	Ogre::MovableObject* movObj = pPlayer.shipNode->getAttachedObject("shipMesh");
	//Get the Ogre Bounding Box
	Ogre::AxisAlignedBox playerAxisBox = movObj->getBoundingBox();
	//Get the size
	Ogre::Vector3 sizeBox = playerAxisBox.getSize();
	//Resize the size (Bullet manual page 18 )
	sizeBox *= 0.96f;

	Ogre::Vector3 v3 = pPlayer.shipNode->getPosition();
	Ogre::Vector3 v32= pPlayer.shipPlaneNode->getPosition();
	Ogre::Vector3 v33= pPlayer.shipPlaneNode->getParentSceneNode()->getPosition();

	//Collision shape for player
	OgreBulletCollisions::BoxCollisionShape *playerBoxShape = new OgreBulletCollisions::BoxCollisionShape(sizeBox);
    // and the Bullet rigid body
    OgreBulletDynamics::RigidBody *playerBody = new OgreBulletDynamics::RigidBody( "playerBody", mWorld );
    playerBody->setShape( pPlayer.shipNode,
             playerBoxShape,
             0.6f,         // dynamic body restitution
             0.6f,         // dynamic body friction
             1.0f,         // dynamic bodymass
			 pPlayer.shipNode->getPosition(),  // starting position of the box
             Ogre::Quaternion(0,0,0,1));// orientation of the box
    mNumEntitiesInstanced++;            
    
	// push the created objects to the deques
    mShapes.push_back(playerBoxShape);
    mBodies.push_back(playerBody);            
}

void icePhysicsMgr::update( Ogre::Real time ){
	//mWorld->stepSimulation( time );
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