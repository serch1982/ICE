#include "Enemies\iceVolcano.h"
#include "iceGame.h"

#define LAVA_UP_TIME 4
#define LAVA_HOLD_TIME 0.5
#define LAVA_DOWN_TIME 2
#define LAVA_TOTAL_DISTACE 126
#define TIME_BETWEEN_ATTACKS 1

iceVolcano::iceVolcano(){
	iceEnemy::iceEnemy();
}

iceVolcano::~iceVolcano(){}

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

	enemyNode->scale(p_Scale);
	enemyNode->rotate(rotation);
	mLavaNode->scale(p_Scale);
	mLavaNode->rotate(rotation);
	/*enemyNode->scale(Ogre::Vector3(2,2,2));
	enemyNode->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(Ogre::Degree(45)));
	mLavaNode->scale(Ogre::Vector3(2,2,2));
	mLavaNode->rotate(Ogre::Vector3(1,0,0),Ogre::Radian(Ogre::Degree(45)));*/
	mLavaNode->translate(Ogre::Vector3(0,-100,0), Ogre::Node::TS_LOCAL);
	mLavaInitialPosition = mLavaNode->_getDerivedPosition();

	//init physics
	icePhysicEntity::initializePhysics("phy_volc"+ entityName.str(), Ogre::Vector3(20,32,20) * p_Scale );
	enemyNode->attachObject(getGeometry()->getMovableObject());

	mAnimations["iddle"] = mesh->getAnimationState( "iddle" );
	mAnimations["iddle"]->setEnabled(true);
	mAnimations["iddle"]->setLoop(true);
	mAnimations["attack"] = mesh->getAnimationState( "attack" );
	mAnimations["attack"]->setLoop(false);

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
	mAnimations["iddle"]->setEnabled(false);
	mAnimations["attack"]->setEnabled(true);
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
				//if(mAnimations["attack"]->getTimePosition() > 1.6)
				//{
					if(mLavaTime<=(LAVA_UP_TIME+LAVA_HOLD_TIME+LAVA_DOWN_TIME))
					{
						if(mLavaTime<=LAVA_UP_TIME)
						{// lava ascendiendo
							Ogre::Vector3 t(0,(LAVA_TOTAL_DISTACE/LAVA_UP_TIME)*p_timeSinceLastFrame,0);
							mLavaNode->translate(t, Ogre::Node::TS_LOCAL);
						}
						else if (mLavaTime > (LAVA_UP_TIME+LAVA_HOLD_TIME))
						{// lava descendiendo
							Ogre::Vector3 t(0,-(LAVA_TOTAL_DISTACE/LAVA_UP_TIME)*p_timeSinceLastFrame,0);
							mLavaNode->translate(t, Ogre::Node::TS_LOCAL);
						}
						//mAnimations["attack"]->addTime(p_timeSinceLastFrame);
						mLavaTime += p_timeSinceLastFrame;
					}
					else
					{// attack ended
						mAnimations["iddle"]->setEnabled(true);
						mAnimations["attack"]->setEnabled(false);

						mAnimations["attack"]->setTimePosition(0);
						mLavaNode->setPosition(mLavaInitialPosition);
						mLavaCreated = false;
						mTimeToNextAtack = TIME_BETWEEN_ATTACKS;
						mTimeToNextAtack += getModifierByLuck(-mTimeToNextAtack/10,mTimeToNextAtack/10);
					}
				//}
				//else
				//{
					mAnimations["attack"]->addTime(p_timeSinceLastFrame);
				//}
			}
			else
			{
				mAnimations["iddle"]->addTime(p_timeSinceLastFrame);

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
			mBillboard->start(enemyNode->_getDerivedPosition());
			iceGame::getGameLog()->logMessage("Enemy killed!");
			icePlayer::getSingletonPtr()->addExperience(mLevel * 10000);
			mAnimDyingTicks++;
			//Dead sequence...
			//When dead sequence finished:
			//mState = INACTIVE;
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
	mBillboard->update(p_timeSinceLastFrame);
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
		mLavaNode->setVisible(false);
		enemyNode->setVisible(false);
	}
}
