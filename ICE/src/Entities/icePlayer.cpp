#include "Entities\icePlayer.h"
#include "Utils/iceDamageTextManager.h"
#include "iceGame.h"

#define CURSOR_PLANE_Z 100
#define CAMERA_PLANE_Z 20
#define CAMERA_ADDED_Y 4
#define CHANNEL_HALF_WIDTH 250
#define CHANNEL_HALF_HEIGHT 200
#define SCROLL_MAX_VELOCITY 100
#define FRAME_MULTIPLICATOR 0.3

#define BARREL_ROLL_TIME 1
#define BARREL_ROLL_DISTANCE 10
#define BRAKE_TIME 3
#define BRAKE_DIVISOR 4
#define SPRINT_TIME 3
#define SPRINT_MULTIMPLICATOR 4


icePlayer::icePlayer():_isShooting(false)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	iceTrajectoryFollower::initialize(sceneManager->getRootSceneNode()->createChildSceneNode("icePlayer"));
	shipMaxVelocity = 20;
	mXUserDeviation = 0;
	mYUserDeviation = 0;

	//Init Scroll
	scrollNode =  mNode->createChildSceneNode();

	//Init cursor
	cursorPlaneNode = scrollNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,CURSOR_PLANE_Z));	
	cursorNode = cursorPlaneNode->createChildSceneNode();
	
	Ogre::BillboardSet* cursorSet = sceneManager->createBillboardSet();
	cursorSet->setMaterialName("cursor");
	Ogre::Billboard* cursorBillboard = cursorSet->createBillboard(0,0,0);
	cursorNode->attachObject(cursorSet);	
	cursorNode->scale(.05,.05,.05);	

	//Init Ship
	shipPlaneNode = scrollNode->createChildSceneNode(Ogre::Vector3(0.0,-CAMERA_ADDED_Y,0.0));
	shipNode = shipPlaneNode->createChildSceneNode();
	rollNode = shipNode->createChildSceneNode();

	//CUIDADO! EL NODO PARA ATACHAR COSAS PARA LA NAVE ES rollNode
	Ogre::Entity* mesh2 = sceneManager->createEntity("shipMesh", "airplane.mesh");
	icePhysicEntity::initialize(mesh2);
	mesh2->setCastShadows(true);
	rollNode->attachObject(mesh2);
	//shipNode->scale(0.1,0.1,0.1);

	// Init camera
	cameraPlaneNode = scrollNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-CAMERA_PLANE_Z));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,0.0));

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

	mMovingUp = false;
	mMovingDown = false;
	mMovingLeft = false;
	mMovingRight = false;

	mLeftBarrelTime = 0;
	mRightBarrelTime = 0;
	mBrakeTime = 0;
	mSprintTime = 0;
	mLog = iceGame::getGameLog();
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
	camera->lookAt(cursorPlaneNode->_getDerivedPosition());
	//camera->setFOVy(Ogre::Degree(20));

	//set some parameters camera-dependent
	mCameraHeight = CAMERA_PLANE_Z * Ogre::Math::Tan(camera->getFOVy());
	mCameraWidth = mCameraHeight * camera->getAspectRatio();

	mMaxScrollX = CHANNEL_HALF_WIDTH - mCameraWidth/2;
	mMaxScrollY = CHANNEL_HALF_HEIGHT - mCameraHeight/2;
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
	//Ogre::Vector3 targetPosition = cursorNode->getPosition() / 4 ;
	////Ogre::Real maxMovement = shipMaxVelocity * frameTime;
	//Ogre::Real maxMovement = getManiobrability() * frameTime;
	//targetPosition.x += -(Ogre::Real)mXUserDeviation*10;
	//targetPosition.y += (Ogre::Real)mYUserDeviation*10;
	//targetPosition.z = 0;
	//Ogre::Vector3 translation = targetPosition - shipNode->getPosition();
	//if (translation.squaredLength() > maxMovement)
	//{
	//	translation.normalise();
	//	translation *= maxMovement;
	//}


	Ogre::Vector3 translation = Ogre::Vector3::ZERO;
	if(mMovingUp)
		translation += Ogre::Vector3::UNIT_Y;
	if(mMovingDown)
		translation += Ogre::Vector3::NEGATIVE_UNIT_Y;
	if(mMovingLeft)
		translation += Ogre::Vector3::UNIT_X;
	if(mMovingRight)
		translation += Ogre::Vector3::NEGATIVE_UNIT_X;

	translation.normalise();
	translation *= getManiobrability() * frameTime;

	shipNode->translate(translation);


	mCameraHeight = CAMERA_PLANE_Z * Ogre::Math::Tan(playerCamera->getFOVy());
	mCameraWidth = mCameraHeight * playerCamera->getAspectRatio();

	std::stringstream str3;
	str3 << (mCameraWidth/2)*FRAME_MULTIPLICATOR;
	iceSdkTray::getInstance()->updateScreenInfo(12,str3.str());
	std::stringstream str4;
	str4 << (mCameraHeight/2)*FRAME_MULTIPLICATOR;
	iceSdkTray::getInstance()->updateScreenInfo(13,str4.str());
}

void icePlayer::updateLookAt(Ogre::Real frameTime)
{
	Ogre::Real x = cursorNode->getPosition().x - shipNode->getPosition().x;
	Ogre::Real y = cursorNode->getPosition().y - shipNode->getPosition().y;
	Ogre::Real z = cursorPlaneNode->getPosition().z;

	shipNode->resetOrientation();
	shipNode->yaw(Ogre::Radian(atan(x/z)));
	shipNode->pitch(-Ogre::Radian(atan(y/z)));
}

void icePlayer::updateScroll(Ogre::Real frameTime)
{
	Ogre::Real scrollX = 0;
	Ogre::Real scrollY = 0;
	Ogre::Vector3 shipPosition = shipNode->getPosition();
	shipPosition.y -= CAMERA_ADDED_Y/2; //Posicion virtual para la direccion vertical
	std::stringstream str;
	str << shipPosition.x;
	iceSdkTray::getInstance()->updateScreenInfo(10,str.str());
	std::stringstream str2;
	str2 << shipPosition.y;
	iceSdkTray::getInstance()->updateScreenInfo(11,str2.str());
	int signX = shipPosition.x >= 0 ? 1 : -1;
	int signY = shipPosition.y >= 0 ? 1 : -1;
	shipPosition.x = abs(shipPosition.x);
	shipPosition.y = abs(shipPosition.y);

	if(shipPosition.x > (mCameraWidth/2)*FRAME_MULTIPLICATOR)
	{
		//scrollAmount en tanto por uno
		Ogre::Real scrollAmount = (shipPosition.x - (mCameraWidth/2)*FRAME_MULTIPLICATOR) / ((mCameraWidth/2)*(1-FRAME_MULTIPLICATOR));
		scrollX = scrollAmount * SCROLL_MAX_VELOCITY * frameTime * signX;
	}

	if(shipPosition.y > (mCameraHeight/2)*FRAME_MULTIPLICATOR)
	{
		//scrollAmount en tanto por uno
		Ogre::Real scrollAmount = (shipPosition.y - (mCameraHeight/2)*FRAME_MULTIPLICATOR) / ((mCameraHeight/2)*(1-FRAME_MULTIPLICATOR));
		scrollY = scrollAmount * SCROLL_MAX_VELOCITY * frameTime * signY;
	}
	scroll(scrollX,scrollY);
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
	Ogre::Real trajectoryUpdate = p_timeSinceLastFrame;
	if(mSprintTime > 0)
	{
		mSprintTime -= p_timeSinceLastFrame;
		trajectoryUpdate *= SPRINT_MULTIMPLICATOR;
	}
	else if(mBrakeTime > 0)
	{
		mBrakeTime -= p_timeSinceLastFrame;
		trajectoryUpdate /= BRAKE_DIVISOR;
	}
	iceTrajectoryFollower::update(trajectoryUpdate);
	if(mLeftBarrelTime > 0)
	{
		updateLeftBarrel(p_timeSinceLastFrame);
	}
	else if(mRightBarrelTime > 0)
	{
		updateRightBarrel(p_timeSinceLastFrame);
	}
	else
	{
		updateShipPosition(p_timeSinceLastFrame);
		rollNode->resetOrientation();
	}
	updateLookAt(p_timeSinceLastFrame);
	updateScroll(p_timeSinceLastFrame);
	iceRPG::update(p_timeSinceLastFrame);
	//addExperience(1000);	
	if (_isShooting){
		shot();
	}						/* Pau */
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
	iceDamageTextManager::getSingletonPtr()->showPlayerDamage(mPhisicEntity,p_iDamage,p_bCritical);
}

void icePlayer::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
	iceDamageTextManager::getSingletonPtr()->showPlayerShieldDamage(mPhisicEntity,p_iDamage,p_bCritical);
}

void icePlayer::showFail(void)
{
	icePlayerStats::getInstance()->addBulletEvaded();
	iceDamageTextManager::getSingletonPtr()->showPlayerMiss(mPhisicEntity);
}

void icePlayer::showLevelUp(unsigned int p_iLevel)
{
	iceDamageTextManager::getSingletonPtr()->showPlayerLevelUp(mPhisicEntity);
}

void icePlayer::setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel)
{
	iceRPG::setWeaponLevel(p_iWeapon, p_iLevel);
	icePlayerStats::getInstance()->setWeaponLevel(p_iWeapon, p_iLevel);
}

Ogre::Vector3 icePlayer::getPosition(void){
	return shipNode->_getDerivedPosition();
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

std::vector<iceBullet*>* icePlayer::getAllBullets(void)
{
	std::vector<iceBullet*>* bullets = new std::vector<iceBullet*>;
	bullets->resize(BULLET_VECTOR_SIZE*3);
	for(unsigned int i=0;i<BULLET_VECTOR_SIZE;i++)
	{
		(*bullets)[i] = mvMachinegunBullets[i];
		(*bullets)[i+BULLET_VECTOR_SIZE] = mvShotgunBullets[i];
		(*bullets)[i+BULLET_VECTOR_SIZE*2] = mvMisilLauncherBullets[i];
	}

	return bullets;
}

void icePlayer::scroll(Ogre::Real x, Ogre::Real y)
{
	if(x != 0 || y != 0)
	{
		Ogre::Vector3 targetScroll = scrollNode->getPosition() + Ogre::Vector3(x,y,0);

		if (abs(targetScroll.x) > mMaxScrollX)
			targetScroll.x = targetScroll.x >= 0 ? mMaxScrollX : -mMaxScrollX;

		if (abs(targetScroll.y) > mMaxScrollY)
			targetScroll.y = targetScroll.y >= 0 ? mMaxScrollY : -mMaxScrollY;
		Ogre::Vector3 finalScroll = targetScroll-scrollNode->getPosition();
		//Pruebas de la nave moviendose alreves del scroll
		scrollNode->setPosition(targetScroll);
		shipNode->translate(-finalScroll/30);
	}
}

//non RPG habilities
void icePlayer::barrelLeft(void)
{
	if(mLeftBarrelTime <= 0)
		mLeftBarrelTime = BARREL_ROLL_TIME;
}

void icePlayer::barrelRight(void)
{
	if(mRightBarrelTime <= 0)
		mRightBarrelTime = BARREL_ROLL_TIME;
}

void icePlayer::sprint(void)
{
	if(mSprintTime <= 0)
	{
		mSprintTime = SPRINT_TIME;
		mBrakeTime = 0; //deactivate brake
	}
}

void icePlayer::brake(void)
{
	if(mBrakeTime <= 0)
	{
		mBrakeTime = BRAKE_TIME;
		mSprintTime = 0; //deactivate sprint
	}
}

void icePlayer::updateLeftBarrel(Ogre::Real pTimeSinceLastEvent)
{
	updateBarrelCommon(&mLeftBarrelTime,pTimeSinceLastEvent,1);
	mLeftBarrelTime -= pTimeSinceLastEvent;
}

void icePlayer::updateRightBarrel(Ogre::Real pTimeSinceLastEvent)
{
	updateBarrelCommon(&mRightBarrelTime,pTimeSinceLastEvent,-1);
	mRightBarrelTime -= pTimeSinceLastEvent;
}

//Direction: 1 = left, -1 = right
void icePlayer::updateBarrelCommon(Ogre::Real* pTime, Ogre::Real pTimeSinceLastEvent, int pDirection)
{
	Ogre::Real lastBarrelRatio = (*pTime)/BARREL_ROLL_TIME;
	(*pTime) -= pTimeSinceLastEvent;
	if((*pTime)<0)
		(*pTime) = 0;
	Ogre::Real barrelRatio = (*pTime)/BARREL_ROLL_TIME;
	Ogre::Real ratioDifference = lastBarrelRatio - barrelRatio;

	Ogre::Vector3 translation = pDirection * ratioDifference * BARREL_ROLL_DISTANCE * Ogre::Vector3::UNIT_X;
	Ogre::Radian rotation = -pDirection * ratioDifference * Ogre::Degree(180) * 4;

	shipNode->translate(translation);
	rollNode->roll(rotation);
}

void icePlayer::setMovingUp(bool pMovingUp)
{
	if(mMovingDown)
		mMovingUp = false;
	else
		mMovingUp = pMovingUp;
}

void icePlayer::setMovingDown(bool pMovingDown)
{
	if(mMovingUp)
		mMovingDown = false;
	else
		mMovingDown = pMovingDown;
}

void icePlayer::setMovingLeft(bool pMovingLeft)
{
	if(mMovingRight)
		mMovingLeft = false;
	else
		mMovingLeft = pMovingLeft;
}

void icePlayer::setMovingRight(bool pMovingRight)
{
	if(mMovingLeft)
		mMovingRight = false;
	else
		mMovingRight = pMovingRight;
}