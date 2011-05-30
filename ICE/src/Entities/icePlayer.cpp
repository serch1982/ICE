#include "Entities\icePlayer.h"
#include "iceGame.h"

icePlayer::icePlayer():_isShooting(false)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	iceTrajectoryFollower::initialize(sceneManager->getRootSceneNode()->createChildSceneNode("icePlayer"));
	shipMaxVelocity = 20;
	mXUserDeviation = 0;
	mYUserDeviation = 0;

	//Init cursor
	cursorPlaneNode = mNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,100.0));	
	cursorNode = cursorPlaneNode->createChildSceneNode();
	
	Ogre::BillboardSet* cursorSet = sceneManager->createBillboardSet();
	cursorSet->setMaterialName("cursor");
	Ogre::Billboard* cursorBillboard = cursorSet->createBillboard(0,0,0);
	cursorNode->attachObject(cursorSet);	
	cursorNode->scale(.05,.05,.05);	

	//Init Ship
	shipPlaneNode = mNode->createChildSceneNode();
	shipNode = shipPlaneNode->createChildSceneNode();

	Ogre::Entity* mesh2 = sceneManager->createEntity("shipMesh", "airplane.mesh");
	mesh2->setCastShadows(true);
	shipNode->attachObject(mesh2);
	//shipNode->scale(0.04,0.09,0.09);

	// Init camera
	cameraPlaneNode = mNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-20.0));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,/*10*/0.0,0.0));

	if( sceneManager->hasCamera("camera") )
		setCamera( sceneManager->getCamera("camera") );
	else
		setCamera( sceneManager->createCamera( "camera" ) );


	//Pau * INITIALIZE BULLETS *----------------------------------------------------------------------------------------//	
	
	/* Create parent node of all bullets */
	mainBulletNode = sceneManager->getRootSceneNode()->createChildSceneNode("bulletMainNode",Ogre::Vector3( 0, 0, 0 ));		
	
	/*Create Machineguns*/
	int i = 0;
	mvMachinegunBullets.resize(BULLET_VECTOR_SIZE);
	mvShotgunBullets.resize(BULLET_VECTOR_SIZE);
	mvMisilLauncherBullets.resize(BULLET_VECTOR_SIZE);
	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{	
		mvMachinegunBullets[i] = new iceBullet();
		mvMachinegunBullets[i]->CreateEntities(sceneManager,mainBulletNode,MACHINEGUN,i);		
	}	
	
	/*Create Shotguns*/	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{	
		mvShotgunBullets[i] = new iceBullet();
		mvShotgunBullets[i]->CreateEntities(sceneManager,mainBulletNode,SHOTGUN,i);		
	}
	
	/*Create MisileLaunchers*/	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i] = new iceBullet();
		mvMisilLauncherBullets[i]->CreateEntities(sceneManager,mainBulletNode,MISILE_LAUNCHER,i);
	}	
	//----------------------------------------------------------------------------------------------------------------------------//

	mLog = Ogre::LogManager::getSingleton().getLog("iceLog.log");
	levelUp();
}

icePlayer::~icePlayer()
{
	for(unsigned int i=0;i<BULLET_VECTOR_SIZE;i++)
	{
		delete mvMachinegunBullets[i];
		delete mvShotgunBullets[i];
		delete mvMisilLauncherBullets[i];
	}

}

void icePlayer::setCamera(Ogre::Camera* camera)
{
	cameraNode->removeAllChildren();
	playerCamera = camera;
	cameraNode->attachObject(camera);
	camera->lookAt(shipNode->convertLocalToWorldPosition(shipNode->getPosition()));
	//camera->setFOVy(Ogre::Degree(20));
}

Ogre::Camera* icePlayer::getCamera()
{
	return playerCamera;
}

void icePlayer::changeWeapon(const int z)
{
	/* Pau * CHANGEWEAPON WITH MOUSE WHEEL */

	static int iLastZ = 0;
	int iCurrentZ = 0;
	bool bWheelUp = false;
	bool bWheelDown = false;

	/* 1st. Check if wheel has been moved "up" or "down" */
	iCurrentZ = z;
	
	if (iCurrentZ > iLastZ)
	{
		bWheelUp = true;
	}
	if (iCurrentZ < iLastZ)
	{
		bWheelDown = true;
	}

	/* 2nd. Change weapon */
	if (bWheelUp)
	{
		if (mCurrentWeapon < 2)  {mCurrentWeapon ++;}
		else if (mCurrentWeapon == 2) {mCurrentWeapon = 0;}
	}
	if (bWheelDown)
	{
		if (mCurrentWeapon > 0)  {mCurrentWeapon --;}
		else if (mCurrentWeapon == 0) {mCurrentWeapon = 2;}
	}

	/* 3rd. Save last mouse wheel Z position value */
	iLastZ = iCurrentZ;

	/* 4th. Show current weapon */
	switch(mCurrentWeapon)
	{
		case  MACHINEGUN:
			iceSdkTray::getInstance()->updateScreenInfo(9,"Machinegun");
			break;
		case SHOTGUN:
			iceSdkTray::getInstance()->updateScreenInfo(9,"Shotgun");
			break;
		case MISILE_LAUNCHER:
			iceSdkTray::getInstance()->updateScreenInfo(9,"Misile launcher");
			break;
	}
	
}

char* icePlayer::getCurrentWeaponName(void){
	switch(mCurrentWeapon)
	{
		case  MACHINEGUN:
			return "Machinegun";
		case SHOTGUN:
			return "Shotgun";
		case MISILE_LAUNCHER:
			return "Misile launcher";
		default:
			return "";
	}
}

void icePlayer::processMouseMoved(const int x, const int y, const int z)
{
	cursorNode->translate(((Ogre::Real)-x)/20,((Ogre::Real)-y)/20,0);		
	changeWeapon(z); /* Pau * Change weapon with mouse wheel */
}

void icePlayer::updateShipPosition(Ogre::Real frameTime)
{
	//updatePosition
	Ogre::Vector3 targetPosition = cursorNode->getPosition() / 4 ;
	//Ogre::Real maxMovement = shipMaxVelocity * frameTime;
	Ogre::Real maxMovement = getManiobrability() * frameTime;
	targetPosition.x += -(Ogre::Real)mXUserDeviation*10;
	targetPosition.y += (Ogre::Real)mYUserDeviation*10;
	targetPosition.z = 0;
	Ogre::Vector3 translation = targetPosition - shipNode->getPosition();
	if (translation.squaredLength() > maxMovement)
	{
		translation.normalise();
		translation *= maxMovement;
	}
	shipNode->translate(translation);

	//lookAt
	Ogre::Real x = cursorNode->getPosition().x - shipNode->getPosition().x;
	Ogre::Real y = cursorNode->getPosition().y - shipNode->getPosition().y;
	Ogre::Real z = cursorPlaneNode->getPosition().z;

	shipNode->resetOrientation();
	shipNode->yaw(Ogre::Radian(atan(x/z)));
	shipNode->pitch(-Ogre::Radian(atan(y/z)));
}
void icePlayer::updateActiveBullets(Ogre::Real p_timeSinceLastFrame)
{
	int i=0;
	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMachinegunBullets[i]->Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvShotgunBullets[i]->Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i]->Update(p_timeSinceLastFrame);
	}
	
}

void icePlayer::addXUserDeviation(int p_iXDeviation)
{
	mXUserDeviation += p_iXDeviation;
	if(mXUserDeviation < -1)
		mXUserDeviation = -1;
	if(mXUserDeviation > 1)
		mXUserDeviation = 1;
}

void icePlayer::addYUserDeviation(int p_iYDeviation)
{
	mYUserDeviation += p_iYDeviation;
	if(mYUserDeviation < -1)
		mYUserDeviation = -1;
	if(mYUserDeviation > 1)
		mYUserDeviation = 1;
}

void icePlayer::update(Ogre::Real p_timeSinceLastFrame)
{
	iceTrajectoryFollower::update(p_timeSinceLastFrame);
	updateShipPosition(p_timeSinceLastFrame);
	iceRPG::update(p_timeSinceLastFrame);
	//addExperience(1000);	
	if (_isShooting){shot();}						/* Pau */
	updateActiveBullets(p_timeSinceLastFrame);	/* Pau */
}

void icePlayer::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic)

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
				if(mvMachinegunBullets[i]->Set(sceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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
				if(mvShotgunBullets[i]->Set(sceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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
				if(mvMisilLauncherBullets[i]->Set(sceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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

void icePlayer::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical)
{
	icePlayerStats::getInstance()->addReceivedDamage(p_iDamage);
}

void icePlayer::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void icePlayer::showFail(void)
{
	icePlayerStats::getInstance()->addBulletEvaded();
}

void icePlayer::showLevelUp(unsigned int p_iLevel)
{
	stringstream strMessage;
	strMessage << "LEVEL UP (" << p_iLevel << ")";
	mLog->logMessage( strMessage.str() );
	icePlayerStats::getInstance()->setPlayerLevel(p_iLevel);

}

void icePlayer::setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel)
{
	iceRPG::setWeaponLevel(p_iWeapon, p_iLevel);
	icePlayerStats::getInstance()->setWeaponLevel(p_iWeapon, p_iLevel);
}

Ogre::Vector3 icePlayer::getPosition(void){
	return shipNode->getPosition();
}


void icePlayer::setIsShooting(bool isShoot){
	_isShooting = isShoot;
}

bool icePlayer::getIsShooting(){
	return _isShooting;
}

int icePlayer::getCurrentWeapon(){
	return mCurrentWeapon;
}

vector<iceBullet*>* icePlayer::getAllBullets(void)
{
	vector<iceBullet*>* bullets = new vector<iceBullet*>;
	bullets->resize(BULLET_VECTOR_SIZE*3);
	for(unsigned int i=0;i<BULLET_VECTOR_SIZE;i++)
	{
		(*bullets)[i] = mvMachinegunBullets[i];
		(*bullets)[i+BULLET_VECTOR_SIZE] = mvShotgunBullets[i];
		(*bullets)[i+BULLET_VECTOR_SIZE*2] = mvMisilLauncherBullets[i];
	}

	return bullets;
}