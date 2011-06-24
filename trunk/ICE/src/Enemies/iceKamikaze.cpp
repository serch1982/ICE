#include "Enemies\iceKamikaze.h"
#include "iceGame.h"

iceKamikaze::iceKamikaze(){
	iceEnemy::iceEnemy();
}

iceKamikaze::~iceKamikaze(){}

bool iceKamikaze::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "kamikaze.mesh");
	mNode->attachObject(mesh);

	//init physics mesh
	icePhysicEntity::initialize(mesh);

	return true;
}

void iceKamikaze::finalize(){
	iceEnemy::finalize();
}

void iceKamikaze::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
}

std::string iceKamikaze::getFunctionStr(){
	return "KamikazeLogic";
}
