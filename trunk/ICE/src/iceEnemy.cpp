#include "iceEnemy.h"

Ogre::NameGenerator iceEnemy::mNameGenerator("Enemy_");

iceEnemy::iceEnemy()
{
	mActivationTime = -1;
	mCurrentTime = 0;
}

iceEnemy::~iceEnemy()
{

}

void iceEnemy::setState(ENEMYSTATE p_iState)
{
	mState = p_iState;
}

int iceEnemy::getState(void)
{
	return mState;
}

void iceEnemy::setPlayer(icePlayer* p_psPlayer)
{
	mPlayer = p_psPlayer;
}

bool iceEnemy::initialize(int id, Ogre::SceneManager* p_psSceneManager, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, ENEMYTYPE p_Type, bool p_isAttachedToPlayer)
{
	mRaySceneQuery = p_psSceneManager->createRayQuery(Ogre::Ray());
	mPlayer = p_psPlayer;
	mType = p_Type;
	mActivationTime = p_fActivationTime;
	mCurrentTime = 0;
	if(p_isAttachedToPlayer)
		enemyNode = mPlayer->getNode()->createChildSceneNode(mNameGenerator.generate());
	else
		enemyNode = mPlayer->getNode()->getParentSceneNode()->createChildSceneNode(mNameGenerator.generate());

	iceTrajectoryFollower::initialize(p_psSceneManager,enemyNode);
	stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();
	Ogre::Entity* mesh;
	switch(mType)
	{
		case MINIMAGMATON:
			mesh = mSceneManager->createEntity(entityName.str(), "minimagmaton.mesh");			
			break;
		case KAMIKAZE:
			mesh = mSceneManager->createEntity(entityName.str(), "kamikaze.mesh");
			break;
		case SMART:
			mesh = mSceneManager->createEntity(entityName.str(), "intelligent.mesh");
			break;
		case VOLCANO:
			mesh = mSceneManager->createEntity(entityName.str(), "volcano.mesh");
			break;
		case MAGMATON:
			mesh = mSceneManager->createEntity(entityName.str(), "magmaton.mesh");
			break;
	}
	mNode->attachObject(mesh);
	mNode->scale(0.1,0.1,0.1);
	mState = INACTIVE;

		//Pau * INITIALIZE BULLETS *----------------------------------------------------------------------------------------//	
	
	/* Create parent node of all bullets */
	enemyBulletNode = mSceneManager->getRootSceneNode()->createChildSceneNode("enemyBullet" + id ,Ogre::Vector3( 0, 0, 0 ));		
	/*Create Machineguns*/
	int i = 0;
	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{		
		mvMachinegunBullets[i].CreateEntities(mSceneManager,enemyBulletNode,MACHINEGUN, i);		
	}	
	
	/*Create Shotguns*/	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{		
		mvShotgunBullets[i].CreateEntities(mSceneManager,enemyBulletNode,SHOTGUN, i);		
	}
	
	/*Create MisileLaunchers*/	
	/*for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i].CreateEntities(mSceneManager,enemyBulletNode,MISILE_LAUNCHER, i);
	}*/	
	//----------------------------------------------------------------------------------------------------------------------------//

	return true;
}

void iceEnemy::finalize()
{

}

void iceEnemy::update(Ogre::Real p_timeSinceLastFrame)
{
	iceRPG::update(p_timeSinceLastFrame);
	
	switch(mState)
	{
		case STOPPED:

			break;
		case FOLLOWING_TRAJECTORY:
			iceTrajectoryFollower::update(p_timeSinceLastFrame);
			break;
		case ATTACKING:
			iceRPG::update(p_timeSinceLastFrame);
			shot();						
			updateActiveBullets(p_timeSinceLastFrame);
			iceTrajectoryFollower::update(p_timeSinceLastFrame);
			break;
		case DEADING:

			iceTrajectoryFollower::update(p_timeSinceLastFrame);
			//Dead sequence...
			//When dead sequence finished:
			//mState = INACTIVE;
			break;
		case INACTIVE:
			//if(checkActivationTime(p_timeSinceLastFrame))
			//{//active
			//	activate();
			//}
			//else
			//{//inactive
			//	mNode->setVisible(false);
			//}
			break;
	}
}

void iceEnemy::activate(void)
{
	mCurrentLife = getMaxLife();
	mNode->setVisible(true);	
}

bool iceEnemy::checkActivationTime(Ogre::Real p_timeSinceLastFrame)
{
	if(mActivationTime < 0)
		return false;

	mCurrentTime += p_timeSinceLastFrame;
	if(mCurrentTime > mActivationTime)
	{
		mActivationTime = -1;
		return true;
	}
	else
	{
		return false;
	}
}

void iceEnemy::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showFail(void)
{
}

void iceEnemy::showLevelUp(unsigned int p_iLevel)
{
}

//get the type of the enemy
iceEnemy::ENEMYTYPE iceEnemy::getType()
{
	return mType;
}
//return if the eneymy whether or not is inside the player camera
bool iceEnemy::isVisiblePlayerCam(){
	Ogre::Camera* playCam = mPlayer->getCamera();
	Ogre::Ray ray(playCam->getPosition(), playCam->getDirection());
	mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) {
        if (itr->movable->getName().compare(enemyNode->getName())!=0) {
            return true;
        }
    }
    return false;

	
}
//return if the eneymy whether or not  is inside the wide (super) camera
bool iceEnemy::isVisibleWideCam(){
	Ogre::Camera *playCam = mPlayer->getCamera();
	Ogre::Ray ray(playCam->getPosition(), playCam->getDirection() * 2);
	mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) {
        if (itr->movable->getName().compare(enemyNode->getName())!=0) {
            return true;
        }
    }
    return false;
}
//return if the eneymy whether or not can shoot
float iceEnemy::rangeAttack(){
	Ogre::Real pZ = mPlayer->getPosition().z;
	Ogre::Real eZ = enemyNode->getPosition().z;
	Ogre::Real tZ = eZ - pZ;
	if ((tZ > 500) ||(tZ < 10)) {
	int i=0;
	}
	return (float)tZ;
}


void iceEnemy::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic)

{
	//Pau * ACTIVATE THE FIRST FREE BULLET OF THE CURRENT WEAPON*-------------------------------------------------------------//
	
	int i = 0;
	bool bFreeBulletFound = false;
	static int iShotSide = 0;	/* One shot is done from the left side of the ship, the next one from the right side, and so on */
	
	switch(p_iWeapon)
	{
		case MACHINEGUN:
		
			while(!bFreeBulletFound)
			{
				if(mvMachinegunBullets[i].Set(mSceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<BULLET_VECTOR_SIZE-1)
					{
						i++;
					}else					
					{
						bFreeBulletFound = true;	/* All bullets have been shooted. Avoid game BUG */
					}
				}
			}
			break;
	
		case SHOTGUN:
		
			while(!bFreeBulletFound)
			{
				if(mvShotgunBullets[i].Set(mSceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<BULLET_VECTOR_SIZE-1)
					{
						i++;
					}else					
					{
						bFreeBulletFound = true;	/* All bullets have been shooted. Avoid game BUG */
					}
				}
			}
			break;

		case MISILE_LAUNCHER:
		
			while(!bFreeBulletFound)
			{
				if(mvMisilLauncherBullets[i].Set(mSceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<BULLET_VECTOR_SIZE-1)
					{
						i++;
					}else					
					{
						bFreeBulletFound = true;	/* All bullets have been shooted. Avoid game BUG */
					}
				}
			}
			break;
	}

	/* Change next shot side */
	if(bFreeBulletFound)
	{
		if(iShotSide == 0)
		{
			iShotSide = 1;
		}else	
		{
			iShotSide = 0;
		}
	}
	//-------------------------------------------------------------------------------------------------//

}

void iceEnemy::updateActiveBullets(Ogre::Real p_timeSinceLastFrame)
{
	int i=0;
	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMachinegunBullets[i].Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvShotgunBullets[i].Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i].Update(p_timeSinceLastFrame);
	}
	
}