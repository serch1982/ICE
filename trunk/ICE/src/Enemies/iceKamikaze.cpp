#include "Enemies\iceKamikaze.h"
#include "iceGame.h"
#include "Utils\iceStrategy.h"

iceKamikaze::iceKamikaze(){
	iceEnemy::iceEnemy();
	mRenewTarget = 100;
}

iceKamikaze::~iceKamikaze(){}

bool iceKamikaze::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position,  p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "kamikaze.mesh");
	enemyNode->attachObject(mesh);

	//init physics
	icePhysicEntity::initializePhysics("phy_kami"+ entityName.str(), Ogre::Vector3(6,6,6));
	enemyNode->attachObject(getGeometry()->getMovableObject());

	//particles
	mParticleFire = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(enemyNode,"ice/iceKamimaze",false);

	//strategy 
	mIceStrategy = iceStrategyPtr(new iceStrategyForward(4));
	return true;
}

void iceKamikaze::finalize(){
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleFire);
	iceEnemy::finalize();
}

void iceKamikaze::update(Ogre::Real p_timeSinceLastFrame){
	
	switch(mState)
	{
		case STOPPED:
			mTrajectory->lookAt();
			break;
		case FOLLOWING_TRAJECTORY:
			mTrajectory->lookAt();
			break;
		case ATTACK: 
			mTrajectory->lookAt();
			enemyNode->translate(mVelocity * p_timeSinceLastFrame);
			mRenewTarget--;
			mParticleFire->stop();
			if( mRenewTarget == 0 ){
				/*Ogre::Vector3 playerPos;
				Ogre::Vector3 kamikazePos;
				//Get Player position relative to his parent
				playerPos = mPlayer->shipNode->getPosition();
				//Convert playerPos to World Coordinates
				mTargetPosition = mPlayer->shipNode->convertLocalToWorldPosition(playerPos);
				//Get Kamikaze WORLD position
				kamikazePos = enemyNode->convertLocalToWorldPosition( kamikazePos );
				//mVelocity = (mTargetPosition-kamikazePos) / 0.7; //Provisional
				mRenewTarget = 100;*/
			}
			break;
		case DYING:
			iceGame::getGameLog()->logMessage("Enemy killed! DYING");
			mAnimDyingTicks++;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{
				mParticleFire->start();
				activate();
			}
			else
			{
				mParticleFire->stop();
				desactivate();
			}
			break;
	}
	mBillboard->update(p_timeSinceLastFrame);
}

std::string iceKamikaze::getFunctionStr(){
	return "KamikazeLogic";
}

void iceKamikaze::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	//iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon, enemyNode->_getDerivedPosition(), -enemyNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
}

void iceKamikaze::setState(ENEMYSTATE p_iState){
	mState = p_iState;
	switch(mState){
		case ATTACK:
			mVelocity = mIceStrategy->move(enemyNode->_getDerivedPosition(), 1);
			mRenewTarget = 100;
			break;
		case DYING:
			mAnimDyingTicks = 0;
			mBillboard->start(enemyNode->_getDerivedPosition());
			break;
		case DEAD:
			icePlayer::getSingletonPtr()->addExperience(mLevel * 10000);
		default:
			break;
	}
}

void iceKamikaze::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
	if(!isAlive()){
		enemyNode->setVisible(false);
	}
}