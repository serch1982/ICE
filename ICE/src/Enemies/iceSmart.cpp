#include "Enemies\iceSmart.h"
#include "iceGame.h"

iceSmart::iceSmart(){
	iceEnemy::iceEnemy();
}

iceSmart::~iceSmart(){}

bool iceSmart::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "intelligent.mesh");
		
	mNode->attachObject(mesh);
	mNode->scale(0.1,0.1,0.1);
	icePhysicEntity::initialize(mesh);

	return true;
}

void iceSmart::finalize(){
	iceEnemy::finalize();
}

void iceSmart::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
}
