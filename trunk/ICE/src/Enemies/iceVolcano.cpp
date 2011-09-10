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
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "volcano.mesh");
	mLavaMesh = sceneManager->createEntity("lava_volcano.mesh");
	enemyNode->attachObject(mesh);
	
	mLavaInitialPosition = Ogre::Vector3(0.0858451,-104.2434,0.675004);

	mLavaNode = enemyNode->createChildSceneNode(mLavaInitialPosition);
	mLavaNode->attachObject(mLavaMesh);

	//mLavaNode->showBoundingBox(true);

	enemyNode->scale(p_Scale);
	enemyNode->rotate(rotation);
	
	//init physics
	icePhysicEntity::initializePhysics("phy_volc"+ entityName.str(), Ogre::Vector3(3,3,3) * p_Scale);
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
			/*if (!isAlive())
				mState = DYING;*/
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
							mLavaNode->translate(0,(LAVA_TOTAL_DISTACE/LAVA_UP_TIME)*p_timeSinceLastFrame,0);
						}
						else if (mLavaTime > (LAVA_UP_TIME+LAVA_HOLD_TIME))
						{// lava descendiendo
							mLavaNode->translate(0,-(LAVA_TOTAL_DISTACE/LAVA_DOWN_TIME)*p_timeSinceLastFrame,0);
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
			/*if (!isAlive()
			)
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

bool iceVolcano::processLavaColision(Ogre::AxisAlignedBox pbox)
{
	if(mLavaMesh->getWorldBoundingBox().intersects(pbox)){
		icePlayer::getSingletonPtr()->addDamage(mAttackDamage,mAttackIsCritic);
		return true;
	}
	else
	{
		return false;
	}
}

