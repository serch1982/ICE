#include "Enemies\iceVolcano.h"
#include "iceGame.h"

iceVolcano::iceVolcano(){
	iceEnemy::iceEnemy();
}

iceVolcano::~iceVolcano(){}

bool iceVolcano::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "volcano.mesh");
	enemyNode->attachObject(mesh);
	enemyNode->scale(0.1,0.1,0.1);
	
	//init physics
	icePhysicEntity::initializePhysics("phy_volc"+ entityName.str(), Ogre::Vector3(3,3,3));
	enemyNode->attachObject(getGeometry()->getMovableObject());

	return true;
}

void iceVolcano::finalize(){
	iceEnemy::finalize();
}

void iceVolcano::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon, enemyNode->_getDerivedPosition(), -enemyNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
}

void iceVolcano::update(Ogre::Real p_timeSinceLastFrame){
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
}

std::string iceVolcano::getFunctionStr(){
	return "VolcanoLogic";
}

