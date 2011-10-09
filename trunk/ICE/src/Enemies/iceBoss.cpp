#include "Enemies\iceBoss.h"
#include "iceGame.h"
#include "Level\iceLevelManager.h"

iceBoss::iceBoss(){
	iceEnemy::iceEnemy();
}

iceBoss::~iceBoss(){
	finalize();
}

bool iceBoss::initialize(int id, Ogre::Vector3 p_Position,  Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mainMesh;
	mainMesh = sceneManager->createEntity(entityName.str(), "magmaton.mesh");
	enemyNode->attachObject(mainMesh);

	/*Ogre::Skeleton* ske = mainMesh->getSkeleton();
	Ogre::Skeleton::BoneIterator bi = ske->getBoneIterator();
	Ogre::String todos ="";
	while (bi.hasMoreElements()) {
		Ogre::Bone* bn = bi.getNext();
		todos += bn->getName() + ";";
		Ogre::String st = bn->getName();
	}*/
	//init physics
	icePhysicEntity::initializePhysics("phy_boss"+ entityName.str(), Ogre::Vector3(10,10,10));
	//add per object 
	enemyNode->attachObject(getGeometry()->getMovableObject()); //must change

	iceAnimationPtr->addAnimation(mainMesh->getAnimationState("CutSceneFinal_Clip"), true, true); 

	enemyNode->yaw(Ogre::Degree(180)); 

	return true;
}

void iceBoss::finalize(){
	iceEnemy::finalize();
}

void iceBoss::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon, enemyNode->_getDerivedPosition(), -enemyNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
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
			icePlayer::getSingletonPtr()->addExperience(mLevel * 10000);
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