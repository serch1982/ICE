#include "Enemies\iceVolcano.h"
#include "iceGame.h"

iceVolcano::iceVolcano(){
	iceEnemy::iceEnemy();
}

iceVolcano::~iceVolcano(){}

bool iceVolcano::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "volcano.mesh");
	mNode->attachObject(mesh);
	mNode->scale(0.1,0.1,0.1);
	
	//init physics mesh
	icePhysicEntity::initialize(mesh);

	return true;
}

void iceVolcano::finalize(){
	iceEnemy::finalize();
}

void iceVolcano::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
}

std::string iceVolcano::getFunctionStr(){
	return "VolcanoLogic";
}

