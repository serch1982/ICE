#include "Enemies\iceKamikaze.h"
#include "iceGame.h"

iceKamikaze::iceKamikaze(){
	iceEnemy::iceEnemy();
	mRenewTarget = 100;
}

iceKamikaze::~iceKamikaze(){
	finalize();
}

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
	Ogre::Real arr[5];
	arr[0] = 0.3;
	arr[1] = 0.6;
	arr[2] = 1.0;
	arr[3] = 2.0;
	int ran = (rand() % 4 + 1) - 1;
	Ogre::Vector3 scale(Ogre::Vector3::UNIT_SCALE * arr[ran] );
	enemyNode->setScale(scale);

	//init physics
	icePhysicEntity::initializePhysics("phy_kami"+ entityName.str(), scale * 5); 
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
			mRenewTarget = 2;
			mTrajectory->lookAt();
			break;
		case ATTACK: 
			enemyNode->translate(mVelocity * p_timeSinceLastFrame);
			mRenewTarget--;
			if( mRenewTarget == 0 ){
				mTrajectory->lookAt();
				mRenewTarget = 2;
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

void iceKamikaze::changeDirection(void){
	enemyNode->setPosition(_lastPosition);
}


void iceKamikaze::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
}

void iceKamikaze::setState(ENEMYSTATE p_iState){
	mState = p_iState;
	switch(mState){
		case ATTACK:
			mVelocity = mIceStrategy->move(enemyNode->_getDerivedPosition(), 1);
			break;
		case DYING:
			mAnimDyingTicks = 0;
			enemyNode->setVisible(false);
			mBillboard->start(enemyNode->_getDerivedPosition());
			break;
		case DEAD:
			enemyNode->setVisible(false);
			giveExperieceToPlayer();
		default:
			break;
	}
}

void iceKamikaze::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
}