#include "Enemies\iceVolcano.h"
#include "iceGame.h"

#define LAVA_UP_TIME 4
#define LAVA_HOLD_TIME 0.5
#define LAVA_DOWN_TIME 2
#define LAVA_TOTAL_DISTACE 70
#define TIME_BETWEEN_ATTACKS 1

iceVolcano::iceVolcano(){
	iceEnemy::iceEnemy();
}

iceVolcano::~iceVolcano(){
	finalize();
}

bool iceVolcano::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, Ogre::Vector3 p_Scale, Ogre::Quaternion rotation, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;

	mTrajectory->setNodeToLookAt(NULL);
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh = sceneManager->createEntity(entityName.str(), "volcano.mesh");
	mLavaMesh = sceneManager->createEntity("lava_volcano.mesh");
	enemyNode->attachObject(mesh);
	
	mLavaInitialPosition = enemyNode->_getDerivedPosition();
	
	entityName << "_lava";
	mLavaNode = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode(entityName.str(),mLavaInitialPosition );
	//  enemyNode->createChildSceneNode(mLavaInitialPosition);
	mLavaNode->attachObject(mLavaMesh);

	//mLavaNode->showBoundingBox(true);

	mScale = p_Scale;

	enemyNode->scale(mScale);
	enemyNode->rotate(rotation);
	mLavaNode->scale(mScale);
	mLavaNode->rotate(rotation);
	mLavaNode->translate(Ogre::Vector3(0,-LAVA_TOTAL_DISTACE,0) * mScale,Ogre::Node::TS_LOCAL);
	mLavaInitialPosition = mLavaNode->getPosition();

	//init physics
	icePhysicEntity::initializePhysics("phy_volc"+ entityName.str(), Ogre::Vector3(20,32,20) * mScale );
	enemyNode->attachObject(getGeometry()->getMovableObject());

	//Animations
	iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("iddle")); 
	iceAnimationPtr->addAnimation(mesh->getAnimationState("attack"),0, true); 

	mLavaCreated = false;
	mAttackDamage = 0;
	mAttackIsCritic = false;
	mLavaTime = 0;
	mTimeToNextAtack = 0;

	return true;
}

void iceVolcano::finalize(){
	iceEnemy::finalize();
}

void iceVolcano::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	mAttackDamage = p_iDamage;
	mAttackIsCritic = p_bCritic;
	iceAnimationPtr->startAnimation("attack");
	mLavaTime = 0;
	mLavaCreated = true;
}

Ogre::Vector3 total; 

void iceVolcano::update(Ogre::Real p_timeSinceLastFrame){
	iceEnemy::update( p_timeSinceLastFrame );

	switch(mState)
	{
		case STOPPED:
			break;
		case FOLLOWING_TRAJECTORY:
			break;
		case ATTACK: 
			iceRPG::update(p_timeSinceLastFrame);
			//gestion del ataque de lava
			if(mLavaCreated)
			{
					if(mLavaTime<=(LAVA_UP_TIME+LAVA_HOLD_TIME+LAVA_DOWN_TIME))
					{
						if(mLavaTime<=LAVA_UP_TIME)
						{// lava ascendiendo
							Ogre::Vector3 t(0,(LAVA_TOTAL_DISTACE/LAVA_UP_TIME)*p_timeSinceLastFrame,0);
							mLavaNode->translate(t * mScale,Ogre::Node::TS_LOCAL);
						}
						else if (mLavaTime > (LAVA_UP_TIME+LAVA_HOLD_TIME))
						{// lava descendiendo
							Ogre::Vector3 t(0,-(LAVA_TOTAL_DISTACE/LAVA_UP_TIME)*p_timeSinceLastFrame,0);
							mLavaNode->translate(t * mScale,Ogre::Node::TS_LOCAL);
						}
						mLavaTime += p_timeSinceLastFrame;
					}
					else
					{// attack ended
						iceAnimationPtr->stopAllAnimations();

						mLavaNode->setPosition(mLavaInitialPosition);
						mLavaCreated = false;
						mTimeToNextAtack = TIME_BETWEEN_ATTACKS;
						mTimeToNextAtack += getModifierByLuck(-mTimeToNextAtack/10,mTimeToNextAtack/10);
					}
			}
			else
			{

				if(mTimeToNextAtack<=0)
				{
					shot();
				}
				else
				{
					mTimeToNextAtack -= p_timeSinceLastFrame;
				}
			}
			break;
		case DYING:
			iceGame::getGameLog()->logMessage("Enemy killed!");
			giveExperieceToPlayer();
			mAnimDyingTicks++;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{
				activate();
			}
			else
			{
				desactivate();
			}
			break;
	}
	
	iceAnimationPtr->update(p_timeSinceLastFrame);
}

std::string iceVolcano::getFunctionStr(){
	return "VolcanoLogic";
}

bool iceVolcano::detectLavaCollision(Ogre::AxisAlignedBox pbox)
{
	if(mLavaMesh->getWorldBoundingBox().intersects(pbox)){
		icePlayer::getSingletonPtr()->addDamage(mAttackDamage,mAttackIsCritic);
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void iceVolcano::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical){
	iceEnemy::showReceivedDamage(p_iDamage, p_bCritical);
	if(!isAlive()){
		Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->setPosition(enemyNode->_getDerivedPosition());
		Ogre::Vector3 scale(.08,.08,.08);
		node->scale(scale);
		iceParticleMgr::getSingletonPtr()->createParticle(node, "explotion_wave",scale);		
		mLavaNode->setVisible(false);
		enemyNode->setVisible(false);
	}
}
