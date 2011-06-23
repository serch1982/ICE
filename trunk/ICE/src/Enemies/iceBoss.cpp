#include "Enemies\iceBoss.h"
#include "iceGame.h"

iceBoss::iceBoss(){
	iceEnemy::iceEnemy();
}

iceBoss::~iceBoss(){}

bool iceBoss::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "magmaton.mesh");
		
	mNode->attachObject(mesh);
	icePhysicEntity::initialize(mesh);

	/*mAttack01 = mesh->getAnimationState( "Attack1" );
	mAttack02 = mesh->getAnimationState( "Attack2" );
	mAttack01->setEnabled( true );
	mAttack02->setEnabled( false );
	mAttack01->setLoop( false );
	mAttack02->setLoop( false );
	mbAnimAttack = true;*/

	return true;
}

void iceBoss::finalize(){
	iceEnemy::finalize();
}

void iceBoss::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
}
