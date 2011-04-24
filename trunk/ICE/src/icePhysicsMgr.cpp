#include "icePhysicsMgr.h"
#include "OgreBulletCollisionsShape.h"
#include <Shapes\OgreBulletCollisionsBoxShape.h>
#include <stdlib.h>



icePhysicsMgr::icePhysicsMgr(){
	mShipCollidesWith = COL_WALL;
	mWallCollidesWith = COL_SHIP;
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
    mDebugDrawer->setDrawWireframe(false);
	mDebugDrawer->setDrawContactPoints(true);

	// enable it if you want to see the Bullet containers
    mWorld->setDebugDrawer(mDebugDrawer);
    mWorld->setShowDebugShapes(false);      
	mWorld->setShowDebugContactPoints( true );
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject(static_cast <Ogre::SimpleRenderable *> (mDebugDrawer));

	// init LOG
	mGameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
	mGameLog->logMessage( "FISICAS: init físicas" );

	mWorld->getBulletDynamicsWorld()->setInternalTickCallback(physicsTickCallback);
	//mWorld->setInternalTickCallback(&icePhysicsMgr::physicsTickCallback);
}

void icePhysicsMgr::setLevel( int p_iCurrentPhase ){

	//TODO: 
	switch( p_iCurrentPhase ){
	case 0:
		setLevel1();
		break;
	case 1:
		setLevel2();
		break;
	case 2:
		setLevel3();
		break;
	default:
		mGameLog->logMessage( "FÍSICAS: nivel no existente" );
		break;
	}
}

void icePhysicsMgr::setLevel1( ){
	stringstream strMessage;
	//Find level root node
	Ogre::Node* playerNode = mSceneMgr->getRootSceneNode()->getChild( "level1" );
	if( !playerNode ){
		mGameLog->logMessage( "FISICAS: no se ha encontrado el nodo del nivel 1" );
	}
	 
	// get the iterator
	Ogre::SceneNode::ChildNodeIterator i = playerNode->getChildIterator();
 
	// Iterate children
	while (i.hasMoreElements()) {
		// Get the child
		Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(i.getNext());
		childNode->showBoundingBox( true );
		string sName = childNode->getName();
		sName;

		//Get the player mesh node
		Ogre::MovableObject* movObj = childNode->getAttachedObject( 0 );
		//Get the Ogre Bounding Box
		Ogre::AxisAlignedBox buildingAxisBox = movObj->getBoundingBox();
		//Get the size
		Ogre::Vector3 sizeBox = buildingAxisBox.getSize();
		//Resize the size (Bullet manual page 18 )
		sizeBox /= 2.0f;
		sizeBox *= 0.96f;

		Ogre::Vector3 v3 = childNode->getPosition();

		// Create a collision shape for buildings
		OgreBulletCollisions::BoxCollisionShape *buildingBoxShape = new OgreBulletCollisions::BoxCollisionShape(sizeBox);
		// and the Bullet rigid body
		OgreBulletDynamics::RigidBody *buildingBody = 
			new OgreBulletDynamics::RigidBody( childNode->getName(), mWorld, COL_WALL, mWallCollidesWith );
		buildingBody->setStaticShape( childNode,
             buildingBoxShape,
             0.6f,         // dynamic body restitution
             0.6f,         // dynamic body friction
			 v3
			);
		mNumEntitiesInstanced++;            
    
		// push the created objects to the deques
		mShapes.push_back(buildingBoxShape);
		mBodies.push_back(buildingBody); 

		mGameLog->logMessage( "FISICAS: Añadido el edificio " + childNode->getName() );
	}
}

void icePhysicsMgr::setLevel2(){
	//Find level root node
	Ogre::Node* playerNode = mSceneMgr->getRootSceneNode()->getChild( "level2" );
	if( !playerNode ){
		mGameLog->logMessage( "FISICAS: no se ha encontrado el nodo del nivel 2" );
	}
	
	// get the iterator
	Ogre::SceneNode::ChildNodeIterator i = playerNode->getChildIterator();
 
	// Iterate children
	while (i.hasMoreElements()) {
		// Get the child
		Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(i.getNext());
		childNode->showBoundingBox( true );
		string sName = childNode->getName();
		sName;

		//Get the player mesh node
		Ogre::MovableObject* movObj = childNode->getAttachedObject( 0 );
		//Get the Ogre Bounding Box
		Ogre::AxisAlignedBox buildingAxisBox = movObj->getBoundingBox();
		//Get the size
		Ogre::Vector3 sizeBox = buildingAxisBox.getSize();
		//Resize the size (Bullet manual page 18 )
		sizeBox /= 2.0f;
		sizeBox *= 0.96f;

		Ogre::Vector3 v3 = childNode->getPosition();

		// Create a collision shape for buildings
		OgreBulletCollisions::BoxCollisionShape *buildingBoxShape = new OgreBulletCollisions::BoxCollisionShape(sizeBox);
		// and the Bullet rigid body
		OgreBulletDynamics::RigidBody *buildingBody = 
			new OgreBulletDynamics::RigidBody( childNode->getName(), mWorld, COL_WALL, mWallCollidesWith );
		buildingBody->setStaticShape( childNode,
             buildingBoxShape,
             0.6f,         // dynamic body restitution
             0.6f,         // dynamic body friction
			 v3
			);
		mNumEntitiesInstanced++;            
    
		// push the created objects to the deques
		mShapes.push_back(buildingBoxShape);
		mBodies.push_back(buildingBody); 

		mGameLog->logMessage( "FISICAS: Añadido el edificio " + childNode->getName() );
	}
}

void icePhysicsMgr::setLevel3(){
	//Find level root node
	Ogre::Node* playerNode = mSceneMgr->getRootSceneNode()->getChild( "level3" );
	if( !playerNode ){
		mGameLog->logMessage( "FISICAS: no se ha encontrado el nodo del nivel 3" );
	}
	
	// get the iterator
	Ogre::SceneNode::ChildNodeIterator i = playerNode->getChildIterator();
 
	// Iterate children
	while (i.hasMoreElements()) {
		// Get the child
		Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(i.getNext());
		childNode->showBoundingBox( true );
		string sName = childNode->getName();
		sName;

		//Get the player mesh node
		Ogre::MovableObject* movObj = childNode->getAttachedObject( 0 );
		//Get the Ogre Bounding Box
		Ogre::AxisAlignedBox buildingAxisBox = movObj->getBoundingBox();
		//Get the size
		Ogre::Vector3 sizeBox = buildingAxisBox.getSize();
		//Resize the size (Bullet manual page 18 )
		sizeBox /= 2.0f;
		sizeBox *= 0.96f;

		Ogre::Vector3 v3 = childNode->getPosition();

		// Create a collision shape for buildings
		OgreBulletCollisions::BoxCollisionShape *buildingBoxShape = new OgreBulletCollisions::BoxCollisionShape(sizeBox);
		// and the Bullet rigid body
		OgreBulletDynamics::RigidBody *buildingBody = 
			new OgreBulletDynamics::RigidBody( childNode->getName(), mWorld, COL_WALL, mWallCollidesWith );
		buildingBody->setStaticShape( childNode,
             buildingBoxShape,
             0.6f,         // dynamic body restitution
             0.6f,         // dynamic body friction
			 v3
			);
		mNumEntitiesInstanced++;            
    
		// push the created objects to the deques
		mShapes.push_back(buildingBoxShape);
		mBodies.push_back(buildingBody); 

		mGameLog->logMessage( "FISICAS: Añadido el edificio " + childNode->getName() );
	}
}

//Add player to physics world
void icePhysicsMgr::addPlayer( icePlayer& pPlayer ){
	
	//Get the player mesh node
	Ogre::MovableObject* movObj = pPlayer.shipNode->getAttachedObject("shipMesh");
	//Get the Ogre Bounding Box
	Ogre::AxisAlignedBox playerAxisBox = movObj->getBoundingBox();
	//Get the size
	Ogre::Vector3 sizeBox = playerAxisBox.getSize();
	//Resize the size (Bullet manual page 18 )
	sizeBox /= 3.0f;
	sizeBox *= 0.96f;

	Ogre::Vector3 vPosition = pPlayer.shipNode->getPosition();

	//Collision shape for player
	OgreBulletCollisions::BoxCollisionShape *playerBoxShape = new OgreBulletCollisions::BoxCollisionShape(sizeBox);
    // and the Bullet rigid body
    OgreBulletDynamics::RigidBody *playerBody = 
		new OgreBulletDynamics::RigidBody( "playerBody", mWorld, COL_SHIP, mShipCollidesWith );
    playerBody->setShape( pPlayer.shipNode,
             playerBoxShape,
             0.6f,         // dynamic body restitution
             0.6f,         // dynamic body friction
             1.0f,         // dynamic bodymass
			 vPosition  // starting position of the box
             //Ogre::Quaternion(0,0,0,1));// orientation of the box
			 );
	// Sets kinematic propierties
	playerBody->setKinematicObject( true );

    mNumEntitiesInstanced++;            
    
	// push the created objects to the deques
    mShapes.push_back(playerBoxShape);
    mBodies.push_back(playerBody);            
}

void icePhysicsMgr::update( Ogre::Real time ){
	mWorld->stepSimulation( time );
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

void icePhysicsMgr::setShowDebug( bool p_bShow ){
	
	mWorld->setShowDebugShapes( p_bShow );
	mWorld->setShowDebugContactPoints( p_bShow );

}

void physicsTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	stringstream strMessage;
	strMessage << "the world just ticked by " << timeStep << " seconds";
	char ptMsg[512];

	Ogre::Log* GameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
    GameLog->logMessage( strMessage.str() );

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				sprintf( ptMsg, "obAName: %s\tobBName: %s", obA->getCollisionShape()->getName(), obB->getCollisionShape()->getName() );
				GameLog->logMessage( ptMsg );
				sprintf( ptMsg, "ptA: X: %.3f\tY: %.3f\tZ: %.3f\tptB: X: %.3f\tY: %.3f\tZ: %.3f", 
					ptA[0], ptA[1], ptA[2], ptB[0], ptB[1], ptB[2] );
				GameLog->logMessage( ptMsg );
			}
		}
	}

}