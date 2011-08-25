#include "Enemies\iceSmart.h"
#include "iceGame.h"

iceSmart::iceSmart(){
	iceEnemy::iceEnemy();
}

iceSmart::~iceSmart(){}

bool iceSmart::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "intelligent.mesh");
	enemyNode->attachObject(mesh);
	//mNode->scale(0.1,0.1,0.1);

	//init physics
	icePhysicEntity::initializePhysics("phy_smart"+ entityName.str(), Ogre::Vector3(10,5.5,4));
	enemyNode->attachObject(getGeometry()->getMovableObject());

	//particles
	mParticleBoom = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(enemyNode,"ice/boom",false);

	//strategy 
	srand(time(NULL));
	bool b = (rand() % 2 + 1) == 1 ? true: false;
	Ogre::Real r = 15 * (Ogre::Math::RangeRandom(20.0,30.0));
	Ogre::Real vel = Ogre::Math::RangeRandom(1.0,2.5);
	mIceStrategy = iceStrategyPtr(new iceStrategySin(r,vel,50, b));

	return true;
}

void iceSmart::finalize(){
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleBoom);
	iceEnemy::finalize();
}

void iceSmart::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon, enemyNode->_getDerivedPosition(), -enemyNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
}

void iceSmart::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );

	switch(mState)
	{
		case STOPPED:
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case FOLLOWING_TRAJECTORY:
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			mTrajectory->lookAt();//TODO
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case ATTACK: 
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
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
}

void iceSmart::changeDirection(void){
	mIceStrategy->reverse();
}

std::string iceSmart::getFunctionStr(){
	return "smartLogic";
}


void iceSmart::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
	if(!mParticleBoom->isPlay() && !isAlive()){
		enemyNode->setVisible(false,false);
		mParticleBoom->start();
	}
}