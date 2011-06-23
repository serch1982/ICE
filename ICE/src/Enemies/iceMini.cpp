#include "Enemies\iceMini.h"
#include "iceGame.h"

iceMini::iceMini(){
	iceEnemy::iceEnemy();
}

iceMini::~iceMini(){}

bool iceMini::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "minimagmaton.mesh");
		
	mNode->attachObject(mesh);
	icePhysicEntity::initialize(mesh);

	return true;
}

void iceMini::finalize(){
	iceEnemy::finalize();
}

void iceMini::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
}
