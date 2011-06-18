#include "Entities\iceEnemy.h"
#include "iceGame.h"

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

bool iceEnemy::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, ENEMYTYPE p_Type, bool p_isAttachedToPlayer)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mRaySceneQuery = sceneManager->createRayQuery(Ogre::Ray());
	mPlayer = p_psPlayer;
	mType = p_Type;
	mActivationTime = p_fActivationTime;
	mCurrentTime = 0;
	Ogre::String name = mNameGenerator.generate();
	if(p_isAttachedToPlayer)
		enemyNode = mPlayer->getNode()->createChildSceneNode(name);
	else
		enemyNode = mPlayer->getNode()->getParentSceneNode()->createChildSceneNode(name);

	iceTrajectoryFollower::initialize(enemyNode);
	stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();
	Ogre::Entity* mesh;
	switch(mType)
	{
		case MINIMAGMATON:
			mesh = sceneManager->createEntity(entityName.str(), "minimagmaton.mesh");			
			break;
		case KAMIKAZE:
			mesh = sceneManager->createEntity(entityName.str(), "kamikaze.mesh");
			break;
		case SMART:
			mesh = sceneManager->createEntity(entityName.str(), "intelligent.mesh");
			break;
		case VOLCANO:
			mesh = sceneManager->createEntity(entityName.str(), "volcano.mesh");
			break;
		case MAGMATON:
			mesh = sceneManager->createEntity(entityName.str(), "magmaton.mesh");
			mAttack01 = mesh->getAnimationState( "Attack1" );
			mAttack02 = mesh->getAnimationState( "Attack2" );
			mAttack01->setEnabled( true );
			mAttack02->setEnabled( false );
			mAttack01->setLoop( false );
			mAttack02->setLoop( false );
			mbAnimAttack = true;
			break;
	}
	mNode->attachObject(mesh);
	if( mType == SMART || mType == VOLCANO )
		mNode->scale(0.1,0.1,0.1);
	mNode->setPosition(p_Position);
	icePhysicEntity::initialize(mesh);

	//Dummy Trajectory
	setTrajectory(new iceTrajectory());
	mTrajectory->setNodeToLookAt(mPlayer->shipNode);


	mState = INACTIVE;
	setLevel(1);

	//	//Pau * INITIALIZE BULLETS *----------------------------------------------------------------------------------------//	
	//
	///* Create parent node of all bullets */
	enemyBulletNode = sceneManager->getRootSceneNode()->createChildSceneNode("enemyBullet" + name,Ogre::Vector3( 0, 0, 0 ));		
	
	//Create Machineguns
	int i = 0;
	mvMachinegunBullets.resize(ENEMY_BULLET_VECTOR_SIZE);
	mvShotgunBullets.resize(ENEMY_BULLET_VECTOR_SIZE);
	mvMisilLauncherBullets.resize(ENEMY_BULLET_VECTOR_SIZE);
	
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{	
		mvMachinegunBullets[i] = new iceBullet();
		mvMachinegunBullets[i]->CreateEntities(sceneManager,enemyBulletNode,MACHINEGUN,i);		
	}	
	
	//Create Shotguns
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{	
		mvShotgunBullets[i] = new iceBullet();
		mvShotgunBullets[i]->CreateEntities(sceneManager,enemyBulletNode,SHOTGUN,i);		
	}
	
	//Create MisileLauncher
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i] = new iceBullet();
		mvMisilLauncherBullets[i]->CreateEntities(sceneManager,enemyBulletNode,MISILE_LAUNCHER,(i+id));
	}	
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
			if (!isAlive())
				mState = DEADING;
			break;
		case FOLLOWING_TRAJECTORY:
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			mTrajectory->lookAt();//TODO
			if (!isAlive())
				mState = DEADING;
			break;
		case ATTACKING: 
			mTrajectory->lookAt(); //TODO
			iceRPG::update(p_timeSinceLastFrame);
			shot();						
			updateActiveBullets(p_timeSinceLastFrame);
			if (!isAlive())
				mState = DEADING;
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			break;
		case DEADING:
			iceGame::getGameLog()->logMessage("Enemy killed!");
			mPlayer->addExperience(mLevel * 10000);
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			//Dead sequence...
			//When dead sequence finished:
			mState = INACTIVE;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{//active
				activate();
			}
			else
			{//inactive
				mNode->setVisible(false);
			}
			break;
	}

	// We are magmaton
	if( mType == 4 ){
		if( mbAnimAttack ){
			mAttack01->addTime( p_timeSinceLastFrame );
			if( mAttack01->hasEnded() ){
				mAttack01->setEnabled( false );
				mAttack02->setEnabled( true );
				mbAnimAttack = !mbAnimAttack;
			}
		}else{
			mAttack02->addTime( p_timeSinceLastFrame );
			if( mAttack02->hasEnded() ){
				mAttack02->setEnabled( false );
				mAttack01->setEnabled( true );
				mbAnimAttack = !mbAnimAttack;
			}
		}
	}
}

void iceEnemy::activate(void)
{
	mCurrentLife = getMaxLife();
	mNode->setVisible(true);
	mState = FOLLOWING_TRAJECTORY;
}

bool iceEnemy::isActive(void)
{
	return (mState != INACTIVE);
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
	stringstream strMessage;
	strMessage << "Adding damage to an enemy = " << p_iDamage << ". Current life = " << getCurrentLife();
	iceGame::getGameLog()->logMessage(strMessage.str());
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
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	switch(p_iWeapon)
	{
		case MACHINEGUN:
		
			while(!bFreeBulletFound)
			{
				if(mvMachinegunBullets[i]->Set(sceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<ENEMY_BULLET_VECTOR_SIZE-1)
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
				if(mvShotgunBullets[i]->Set(sceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<ENEMY_BULLET_VECTOR_SIZE-1)
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
				if(mvMisilLauncherBullets[i]->Set(sceneManager,enemyNode,p_iDamage,p_bCritic,iShotSide))
				{
					bFreeBulletFound = true;					
					
				}else
				{		
					if (i<ENEMY_BULLET_VECTOR_SIZE-1)
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
	
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{
		mvMachinegunBullets[i]->Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{
		mvShotgunBullets[i]->Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < ENEMY_BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i]->Update(p_timeSinceLastFrame);
	}
	
}

vector<iceBullet*>* iceEnemy::getAllBullets(void)
{
	vector<iceBullet*>* bullets = new vector<iceBullet*>;
	bullets->resize(ENEMY_BULLET_VECTOR_SIZE*3);
	for(unsigned int i=0;i<ENEMY_BULLET_VECTOR_SIZE;i++)
	{
		(*bullets)[i] = mvMachinegunBullets[i];
		(*bullets)[i+ENEMY_BULLET_VECTOR_SIZE] = mvShotgunBullets[i];
		(*bullets)[i+ENEMY_BULLET_VECTOR_SIZE*2] = mvMisilLauncherBullets[i];
	}

	return bullets;
}

Ogre::Vector3 iceEnemy::getWorldPosition(void)
{
	return enemyNode->_getDerivedPosition();
}