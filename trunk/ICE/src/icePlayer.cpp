#include <icePlayer.h>

icePlayer::icePlayer()
{

}

icePlayer::~icePlayer()
{

}

bool icePlayer::initialize(Ogre::SceneManager* p_psSceneManager, Ogre::SceneNode* p_psNode)
{
	iceTrajectoryFollower::initialize(p_psSceneManager,p_psNode);
	shipMaxVelocity = 20;
	mXUserDeviation = 0;
	mYUserDeviation = 0;

	//Init cursor
	cursorPlaneNode = mNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,100.0));	
	cursorNode = cursorPlaneNode->createChildSceneNode();
	
	Ogre::BillboardSet* cursorSet = p_psSceneManager->createBillboardSet();
	cursorSet->setMaterialName("cursor");
	Ogre::Billboard* cursorBillboard = cursorSet->createBillboard(0,0,0);
	cursorNode->attachObject(cursorSet);	
	cursorNode->scale(.05,.05,.05);	

	//Init Ship
	shipPlaneNode = mNode->createChildSceneNode();
	shipNode = shipPlaneNode->createChildSceneNode();

	Ogre::Entity* mesh2 = mSceneManager->createEntity("shipMesh", "nave.mesh");
	mesh2->setCastShadows(true);
	shipNode->attachObject(mesh2);
	shipNode->scale(0.04,0.09,0.09);

	// Init camera
	cameraPlaneNode = mNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-150.0));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,/*10*/0.0,0.0));

	if( mSceneManager->hasCamera("PlayerCam") )
		setCamera( mSceneManager->getCamera("PlayerCam") );
	else
		setCamera( mSceneManager->createCamera( "PlayerCam" ) );


	//Pau * INITIALIZE BULLETS *----------------------------------------------------------------------------------------//	
	
	/* Create parent node of all bullets */
	mainBulletNode = mSceneManager->getRootSceneNode()->createChildSceneNode("bulletMainNode",Ogre::Vector3( 0, 0, 0 ));		
	
	/*Create Machineguns*/
	int i = 0;
	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{		
		mvMachinegunBullets[i].CreateEntities(mSceneManager,mainBulletNode,MACHINEGUN,i);		
	}	
	
	/*Create Shotguns*/	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{		
		mvShotgunBullets[i].CreateEntities(mSceneManager,mainBulletNode,SHOTGUN,i);		
	}
	
	/*Create MisileLaunchers*/	
	for(i = 0; i < BULLET_VECTOR_SIZE; i++)
	{
		mvMisilLauncherBullets[i].CreateEntities(mSceneManager,mainBulletNode,MISILE_LAUNCHER,i);
	}	
	//----------------------------------------------------------------------------------------------------------------------------//


	mLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );

	return true;
}

void icePlayer::setCamera(Ogre::Camera* camera)
{
	cameraNode->removeAllChildren();
	playerCamera = camera;
	cameraNode->attachObject(camera);
	camera->lookAt(cursorNode->convertLocalToWorldPosition(cursorNode->getPosition()));
	camera->setFOVy(Ogre::Degree(20));
}

Ogre::Camera* icePlayer::getCamera()
{
	return playerCamera;
}

void icePlayer::changeWeapon(const OIS::MouseEvent &arg)
{
	/* Pau * CHANGEWEAPON WITH MOUSE WHEEL */

	static int iLastZ = 0;
	int iCurrentZ = 0;
	bool bWheelUp = false;
	bool bWheelDown = false;

	/* 1st. Check if wheel has been moved "up" or "down" */
	iCurrentZ = arg.state.Z.abs;
	
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
			iceDebugScreen::getInstance()->updateChivato(9,"Machinegun");
			break;
		case SHOTGUN:
			iceDebugScreen::getInstance()->updateChivato(9,"Shotgun");
			break;
		case MISILE_LAUNCHER:
			iceDebugScreen::getInstance()->updateChivato(9,"Misile launcher");
			break;
	}
	
}
void icePlayer::processMouseMoved(const OIS::MouseEvent &arg)
{
	cursorNode->translate(((Ogre::Real)-arg.state.X.rel)/20,((Ogre::Real)-arg.state.Y.rel)/20,0);		
	changeWeapon(arg); /* Pau * Change weapon with mouse wheel */
}

void icePlayer::updateShipPosition(Ogre::Real frameTime)
{
	//updatePosition
	Ogre::Vector3 targetPosition = cursorNode->getPosition();
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
void icePlayer::finalize()
{

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

void icePlayer::update(Ogre::Real p_timeSinceLastFrame,bool Shooting)
{
	iceTrajectoryFollower::update(p_timeSinceLastFrame);
	updateShipPosition(p_timeSinceLastFrame);
	iceRPG::update(p_timeSinceLastFrame);
	addExperience(1000);	
	if (Shooting){shot();}						/* Pau */
	updateActiveBullets(p_timeSinceLastFrame);	/* Pau */
}

void icePlayer::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic)

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
				if(mvMachinegunBullets[i].Set(mSceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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
				if(mvShotgunBullets[i].Set(mSceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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
				if(mvMisilLauncherBullets[i].Set(mSceneManager,shipNode,p_iDamage,p_bCritic,iShotSide))
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
	iceCounters::instance()->addReceivedDamage(p_iDamage);
}

void icePlayer::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void icePlayer::showFail(void)
{
	iceCounters::instance()->addBulletEvaded();
}

void icePlayer::showLevelUp(unsigned int p_iLevel)
{
	stringstream strMessage;
	strMessage << "LEVEL UP (" << p_iLevel << ")";
	mLog->logMessage( strMessage.str() );
	iceCounters::instance()->setPlayerLevel(p_iLevel);

}

void icePlayer::setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel)
{
	iceRPG::setWeaponLevel(p_iWeapon, p_iLevel);
	iceCounters::instance()->setWeaponLevel(p_iWeapon, p_iLevel);
}

Ogre::Vector3 icePlayer::getPosition(void){
	return shipNode->getPosition();
}
