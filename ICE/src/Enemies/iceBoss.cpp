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

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "magmaton.mesh");
	mNode->attachObject(mesh);

	//init physics
	icePhysicEntity::initializePhysics("phy_boss"+ entityName.str(), Ogre::Vector3(10,10,10));
	//add per object 
	mNode->attachObject(getGeometry()->getMovableObject()); //must change

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

	switch(mState)
	{
		case STOPPED:
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case FOLLOWING_TRAJECTORY:
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			mTrajectory->lookAt();//TODO
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case ATTACK: 
			mTrajectory->lookAt(); //TODO
			iceRPG::update(p_timeSinceLastFrame);
			shot();
			/*if (!isAlive())
				mState = DYING;*/
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			break;
		case DYING:
			mBillboard->start(enemyNode->_getDerivedPosition());
			iceGame::getGameLog()->logMessage("Enemy killed!");
			mPlayer->addExperience(mLevel * 10000);
			mAnimDyingTicks++;
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			//Dead sequence...
			//When dead sequence finished:
			//mState = INACTIVE;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{//active
				activate();
			}
			else
			{//inactive
				desactivate();
			}
			break;
	}

		
	// We are magmaton
	/*if( mType == 4 ){
		if( mbAnimAttack ){
			mAttack01->addTime( p_timeSinceLastFrame );
			if( mAttack01->hasEnded() ){
				mAttack01->setEnabled( false );
				mAttack02->setEnabled( true );
				mbAnimAttack = !mbAnimAttack;
			}
		}else{
			mAttack02->addTime( p_timeSinceLastFrame );
			if( mAttack02->hasEnded() ){
				mAttack02->setEnabled( false );
				mAttack01->setEnabled( true );
				mbAnimAttack = !mbAnimAttack;
			}
		}
	}*/
}

std::string iceBoss::getFunctionStr(){
	return "BossLogic";
}