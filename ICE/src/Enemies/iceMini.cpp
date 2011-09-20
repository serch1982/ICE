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
	skeleton = mesh->getSkeleton();
	
	/*Ogre::Skeleton::BoneIterator bi = ske->getBoneIterator();
	while (bi.hasMoreElements()) {
		Ogre::Bone* bn = bi.getNext();
		Ogre::String st = bn->getName();
	}*/
	iceAnimationPtr->addAnimation(mesh->getAnimationState("iddle"), true, true); 

	//init physics
	icePhysicEntity::initializePhysics("phy_mini"+ entityName.str(), Ogre::Vector3(3.2,7,2));
	enemyNode->attachObject(getGeometry()->getMovableObject());

	//particles
	mParticleFire = iceParticleMgr::getSingletonPtr()->createPartAttachToBone(mesh,"right_tibia","ice/fireDown",false);

	//strategy 
	srand(time(NULL));
	bool b = (rand() % 2 + 1) == 1 ? true: false;
	Ogre::Real r = 10 * (Ogre::Math::RangeRandom(25.0,45.0));
	Ogre::Real vel = Ogre::Math::RangeRandom(1.0,2.5);
	mIceStrategy = iceStrategyPtr(new iceStrategyCircle(vel,20,r, b ));
	mIceStrategyMini = iceStrategyPtr(new iceStrategyForward(5));
	return true;
}

void iceMini::finalize(){
	mIceStrategyMini.reset();
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
			break;
		case FOLLOWING_TRAJECTORY:
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			mTrajectory->lookAt();
			break;
		case ATTACK: 
			mTrajectory->lookAt(); 
			enemyNode->translate(mIceStrategy->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			enemyNode->translate(mIceStrategyMini->move(enemyNode->_getDerivedPosition(), p_timeSinceLastFrame));
			shot(); 
			break;
		case DYING:
			iceGame::getGameLog()->logMessage("Enemy killed!");
			mAnimDyingTicks++;
			mParticleFire->stop();
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
	iceAnimationPtr->update(p_timeSinceLastFrame);
}

std::string iceMini::getFunctionStr(){
	return "miniLogic";
}

void iceMini::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	Ogre::Vector3 posb =  skeleton->getBone("Left_leg")->getPosition();
	Ogre::Vector3 posbx = enemyNode->_getDerivedPosition() + posb;
	iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon,posbx, -enemyNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
}

void iceMini::changeDirection(void){
	mIceStrategy->reverse();
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
			enemyNode->setVisible(false);
			giveExperieceToPlayer();
			break;
		default:
			break;
	}
}

void iceMini::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
	if(!isAlive()){
		enemyNode->setVisible(false);
		Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode(enemyNode->getName() + "_exp");
		node->setPosition(enemyNode->_getDerivedPosition());
		iceParticleMgr::getSingletonPtr()->createParticle(node, "ice/crashboom");	
	}
}