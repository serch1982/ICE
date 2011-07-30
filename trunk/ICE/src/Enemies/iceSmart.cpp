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

	//strategy 
	mIceStrategy = iceStrategyPtr(new iceStrategySin(30,20, true));

	return true;
}

void iceSmart::finalize(){
	iceEnemy::finalize();
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
std::string iceSmart::getFunctionStr(){
	return "smartLogic";
}
