#include "Enemies\iceMini.h"
#include "iceGame.h"


iceMini::iceMini(){
	iceEnemy::iceEnemy();
}

iceMini::~iceMini(){}

bool iceMini::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "minimagmaton.mesh");
	enemyNode->attachObject(mesh);
	Ogre::Skeleton* ske = mesh->getSkeleton();
	//mIddle = mesh->getAnimationState( "iddle" );
	/*mIddle2 = mesh->getAnimationState( "iddle2" );
	mIddle2->setEnabled( true );
	mIddle2->setLoop( true );*/

	//init physics
	icePhysicEntity::initializePhysics("phy_mini"+ entityName.str(), Ogre::Vector3(3.2,7,2));
	enemyNode->attachObject(getGeometry()->getMovableObject());

	//particles
	mParticleFire = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(enemyNode,"ice/fireminimagmaton",false);
	mParticleBoom = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(enemyNode,"ice/boom",false);

	//strategy 
	mIceStrategy = iceStrategyPtr(new iceStrategyCircle(1.5,25, true));
	return true;
}

void iceMini::finalize(){
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleBoom);
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleFire);
	iceEnemy::finalize();
}

void iceMini::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );
	Ogre::Vector3 enemyPos = enemyNode->convertLocalToWorldPosition( enemyNode->getPosition() );
	switch(mState)
	{
		case STOPPED:
			mTrajectory->lookAt();
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case FOLLOWING_TRAJECTORY:
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			mTrajectory->lookAt();//TODO
			//enemyNode->setPosition(mIceStrategy->move(Ogre::Vector3(0,0,0), enemyNode->_getDerivedPosition(), 1, Ogre::Vector3(1,0,0)));
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case ATTACK: 
			mTrajectory->lookAt(); //TODO  * p_timeSinceLastFrame
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			//if(mIddle->hasEnded())
			//{
			//	mIddle->setEnabled(false);
			//	mIddle2->setEnabled(true);
			//}
			//else
			//{
			//	mIddle->addTime(p_timeSinceLastFrame);
			//}

			//if(mIddle2->hasEnded())
			//{
			//	mIddle2->setEnabled(false);
			//	mIddle->setEnabled(true);
			//}
			//else
			//{
				//mIddle2->addTime(p_timeSinceLastFrame);
			//}
			shot(); 
			/*if (!isAlive())
				mState = DYING;*/
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			break;
		case DYING:
			iceGame::getGameLog()->logMessage("Enemy killed!");
			mAnimDyingTicks++;
			mParticleFire->stop();
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			//Dead sequence...
			//When dead sequence finished:
			//mState = INACTIVE;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{//active
				mParticleFire->start();
				activate();
			}
			else
			{//inactive
				mParticleFire->stop();
				desactivate();
			}
			break;
	}
	mBillboard->update(p_timeSinceLastFrame);
}

std::string iceMini::getFunctionStr(){
	return "miniLogic";
}

void iceMini::setState(ENEMYSTATE p_iState){
	mState = p_iState;
	bool b;
	switch(mState){
	case INACTIVE:
		b = isAlive();
		break;
	case FOLLOWING_TRAJECTORY:
		b = isAlive();
		break;
		case DYING:
			mAnimDyingTicks = 0;
			mBillboard->start(enemyNode->_getDerivedPosition());
			break;
		case DEAD:
			icePlayer::getSingletonPtr()->addExperience(mLevel * 10000);
			break;
		default:
			break;
	}
}

void iceMini::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
	if(!mParticleBoom->isPlay() && !isAlive()){
		enemyNode->setVisible(false,false);
		mParticleBoom->start();
	}
}