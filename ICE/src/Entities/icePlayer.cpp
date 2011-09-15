#include "Entities\icePlayer.h"
#include "Utils\iceDamageTextManager.h"
#include "iceGame.h"
#include "Particle\iceParticleMgr.h"
#include "Entities\iceBulletMgr.h"
#include "PostProcess\icePostProcessManager.h"

#define VIRTUAL_CAMERA_SIZE_X 1000
#define VIRTUAL_CAMERA_SIZE_Y 1000
#define VIRTUAL_CAMERA_SIZE_Z 1500

#define CURSOR_PLANE_Z 100
#define CAMERA_PLANE_Z 20
#define CAMERA_ADDED_Y 4
#define CHANNEL_HALF_WIDTH 250
#define CHANNEL_HALF_HEIGHT 200
#define SCROLL_MAX_VELOCITY 100
#define FRAME_MULTIPLICATOR 0.3

#define BARREL_ROLL_TIME 1
#define BARREL_ROLL_DISTANCE_MULTIPLIER 1.5
#define BRAKE_TIME 3
#define BRAKE_DIVISOR 4
#define SPRINT_TIME 3
#define SPRINT_MULTIMPLICATOR 2

#define INVULNERABLE_TIME 2
#define BLINK_TIME 0.2

#ifdef _DEBUG 
	#define VELOCITY 0.08
#else
	#define VELOCITY 0.05
#endif
#define MAX_VELOCITY 5

#define ROLL_VELOCITY 0.1
#define MAX_ROLL 20

#define LOOK_AT_FAR_FACTOR 1000

// BEGIN SINGLETON
template<> icePlayer* Ogre::Singleton<icePlayer>::ms_Singleton = 0;

icePlayer* icePlayer::getSingletonPtr(void)
{
    return ms_Singleton;
}
icePlayer& icePlayer::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

icePlayer::icePlayer():_isShooting(false)
{
	initPlayer();
	mLog = iceGame::getGameLog();
}

icePlayer::~icePlayer()
{
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleTurboLeft);
	iceParticleMgr::getSingletonPtr()->removeParticle(mParticleTurboRight);
	virtualCam.reset();
	iceAnimationPtr.reset();
	icePhysicEntity::finalizePhysics();
}

void icePlayer::initPlayer(){
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	iceTrajectoryFollower::initialize(sceneManager->getRootSceneNode()->createChildSceneNode("icePlayer"));
	shipMaxVelocity = 20;
	mXUserDeviation = 0;
	mYUserDeviation = 0;

	_velocityX = MAX_VELOCITY;
	_velocityY = MAX_VELOCITY;
	_rolling = 0;
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
	_lastPosition = Ogre::Vector3(0,0,0);
	//physics
	icePhysicEntity::initializePhysics("phy_player", Ogre::Vector3(5.3,2.8,4.7));
	shipNode->attachObject(getGeometry()->getMovableObject());
		
	rollNode = shipNode->createChildSceneNode();

	//CUIDADO! EL NODO PARA ATACHAR COSAS PARA LA NAVE ES rollNode

	//particles
	mParticleTurboLeft = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(rollNode, Ogre::Vector3(1.1,-0.1,-.2),"ice/icePlayerTurbo",true);
	mParticleTurboRight = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(rollNode, Ogre::Vector3(-1.1,-0.1,-.2),"ice/icePlayerTurbo",true);

	Ogre::Entity* mesh2 = sceneManager->createEntity("shipMesh", "airplane.mesh");
	mesh2->setCastShadows(true);
	rollNode->attachObject(mesh2);
	//shipNode->scale(0.1,0.1,0.1);

	//init animations
	iceAnimationPtr = iceAnimationMgrPtr(new iceAnimationMgr());
	iceAnimationPtr->addAnimation(mesh2->getAnimationState("queja"), true, true); 
	

	// Init camera
	cameraPlaneNode = scrollNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-CAMERA_PLANE_Z));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,0.0));

	//virtual boundingbox
	snVirtualCam = shipNode->createChildSceneNode(Ogre::Vector3(0,0,((VIRTUAL_CAMERA_SIZE_Z / 2) - CAMERA_PLANE_Z ))); 
	virtualCam = iceGeometryPtr(new iceGeometry(Ogre::Vector3(VIRTUAL_CAMERA_SIZE_X,VIRTUAL_CAMERA_SIZE_Y,VIRTUAL_CAMERA_SIZE_Z), "virtualCam"));
	snVirtualCam->attachObject(virtualCam->getMovableObject());
	

	if( sceneManager->hasCamera("camera") )
		setCamera( sceneManager->getCamera("camera") );
	else{
		iceGame::createCamera();
		setCamera( sceneManager->getCamera("camera") );
	}

	mMovingUp = false;
	mMovingDown = false;
	mMovingLeft = false;
	mMovingRight = false;

	mLeftBarrelTime = 0;
	mRightBarrelTime = 0;
	mBrakeTime = 0;
	mSprintTime = 0;

	mInvulnerable = false;
	mInvulnerableTime = 0;
	mBlinkTime = 0;
	mBlinkVisible = true;
}

Ogre::AxisAlignedBox icePlayer::getVitualCamBBox(void) {
	return virtualCam->getWorldBoundingBox(snVirtualCam->_getDerivedPosition());
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
	static int lastZ = 0;
	
	if(z != lastZ)
	{
		int increment = 1;
		if (z < lastZ)
		{
			increment = 2;
		}

		unsigned int nextWeapon = (mCurrentWeapon+increment)%3;

		while(mWeaponLevel[nextWeapon] == 0)
			nextWeapon = (nextWeapon+increment)%3;

		mCurrentWeapon = nextWeapon;
		iceGame::getUI()->getHUD()->setWeapon(mCurrentWeapon+1,mWeaponLevel[mCurrentWeapon]);

		lastZ = z;
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
	//restrict the cursor inside the screen
	Ogre::Real h = 50 ;
	Ogre::Real w = iceGame::getRenderWindow()->getWidth() < 800 ? 65 :  (iceGame::getRenderWindow()->getWidth() * 7.1) / 100.0;
	Ogre::Vector3 ori = cursorNode->getPosition();
	Ogre::Real mx = Ogre::Math::Abs(ori.x);
	Ogre::Real my = Ogre::Math::Abs(ori.y);
	if(mx > w) {
		Ogre::Real amx = mx - w;
		cursorNode->translate(ori.x > 0 ? -amx : amx,0,0);
	}
	if(my > h) {
		Ogre::Real amy = my - h;
		cursorNode->translate(0,ori.y > 0 ? -amy : amy,0);
	}
	iceSdkTray::getInstance()->updateScreenInfo( 16, Ogre::StringConverter::toString(ori.x) + " - " + Ogre::StringConverter::toString(ori.y));
	changeWeapon(z); /* Pau * Change weapon with mouse wheel */
}

void icePlayer::updateShipPosition(Ogre::Real frameTime)
{
	_lastPosition = shipNode->getPosition();

	Ogre::Vector3 camPos = cameraPlaneNode->getPosition();
	Ogre::Vector3 shipPos = shipNode->getPosition();
	
	Ogre::Vector3 translation = Ogre::Vector3::ZERO;

	Ogre::Real spcam = (shipPos.z - camPos.z) ; 
	if ((spcam < CAMERA_PLANE_Z) || (shipPos.z < 0)) 
		translation += Ogre::Vector3::UNIT_Z;

	if(mMovingUp){
		_velocityY -= VELOCITY ;
		if(_velocityY <= 1) _velocityY = 1;
		translation += Ogre::Vector3::UNIT_Y;
	}
	if(mMovingDown){
		_velocityY -= VELOCITY ;
		if(_velocityY <= 1) _velocityY = 1;
		translation += Ogre::Vector3::NEGATIVE_UNIT_Y;
	}
	if(mMovingLeft){
		_velocityX -= VELOCITY ;
		if(_velocityX <= 1) _velocityX = 1;
		translation += Ogre::Vector3::UNIT_X;
	}
	if(mMovingRight){
		_velocityX -= VELOCITY ;
		if(_velocityX <= 1) _velocityX = 1;
		translation += Ogre::Vector3::NEGATIVE_UNIT_X;
	}
	Ogre::Real divVel =((_velocityX * _velocityY)/MAX_VELOCITY);
	divVel = divVel < 1 ? 1: divVel;
	iceSdkTray::getInstance()->updateScreenInfo( 8, Ogre::StringConverter::toString(_velocityX) + " -- " + Ogre::StringConverter::toString(_velocityY)+ " -- " + Ogre::StringConverter::toString(divVel));
	translation.normalise();
	translation *=(getManiobrability() /divVel ) * frameTime;

	shipNode->translate(translation);
	shipNode->resetOrientation();
	

	mCameraHeight = CAMERA_PLANE_Z * Ogre::Math::Tan(playerCamera->getFOVy());
	mCameraWidth = mCameraHeight * playerCamera->getAspectRatio();

	std::stringstream str3;
	str3 << (mCameraWidth/2)*FRAME_MULTIPLICATOR;
	iceSdkTray::getInstance()->updateScreenInfo(12,str3.str());
	std::stringstream str4;
	str4 << (mCameraHeight/2)*FRAME_MULTIPLICATOR;
	iceSdkTray::getInstance()->updateScreenInfo(13,str4.str());
	std::stringstream str5;
	str5 << mLevel;
	iceSdkTray::getInstance()->updateScreenInfo(14,str5.str());
}


void icePlayer::updateLookAt(Ogre::Real frameTime)
{
	Ogre::Real rollVelocity = ROLL_VELOCITY * getManiobrability();
	if(mMovingLeft){
		_rolling = _rolling - rollVelocity;
		if(_rolling < -MAX_ROLL) _rolling  = -MAX_ROLL;
	}
	if(mMovingRight){
		_rolling = _rolling + rollVelocity;
		if(_rolling > MAX_ROLL) _rolling  = MAX_ROLL;
	}
	if ((!mMovingRight) && (!mMovingLeft)){
		if(_rolling != 0)
		if (_rolling > 0)
		{
			_rolling = _rolling - rollVelocity;
			_rolling = _rolling > 0 ? _rolling : 0;
		}
		else
		{
			_rolling = _rolling + rollVelocity;
			_rolling = _rolling < 0 ? _rolling : 0;
		}
	}
		//MAX_ROLL
	Ogre::Real x = cursorNode->getPosition().x*LOOK_AT_FAR_FACTOR - shipNode->getPosition().x;
	Ogre::Real y = cursorNode->getPosition().y*LOOK_AT_FAR_FACTOR - shipNode->getPosition().y;
	Ogre::Real z = (CURSOR_PLANE_Z+CAMERA_PLANE_Z)*LOOK_AT_FAR_FACTOR - CAMERA_PLANE_Z;

	shipNode->resetOrientation();
	shipNode->yaw(Ogre::Radian(atan(x/z)));
	shipNode->pitch(-Ogre::Radian(atan(y/z)));
	shipNode->roll(Ogre::Radian(_rolling * ( Ogre::Math::PI / 180)));
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
	else
	{
		icePostProcessManager::getSingleton().enableSoftBlur();
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
	//restrict the ship inside the screen 
	Ogre::Real h = 11 ;
	Ogre::Real w = 12;
	Ogre::Vector3 ori = shipNode->getPosition();
	Ogre::Real mx = Ogre::Math::Abs(ori.x);
	Ogre::Real my = Ogre::Math::Abs(ori.y);
	if(mx > w) {
		Ogre::Real amx = mx - w;
		shipNode->translate(ori.x > 0 ? -amx : amx,0,0);
	}
	if(my > h) {
		Ogre::Real amy = my - h;
		shipNode->translate(0,ori.y > 0 ? -amy : amy,0);
	}
	iceSdkTray::getInstance()->updateScreenInfo( 17, Ogre::StringConverter::toString(ori.x) + " - " + Ogre::StringConverter::toString(ori.y));
	//
	iceRPG::update(p_timeSinceLastFrame);
	//addExperience(1000);	//TODO borrar
	if (_isShooting){
		shot();
	}

	if(mInvulnerable)
	{
		if(mInvulnerableTime > 0)
		{
			if(mBlinkTime <= 0)
			{
				mBlinkVisible = !mBlinkVisible;
				shipNode->setVisible(mBlinkVisible);
				mBlinkTime = BLINK_TIME;
			}
			mInvulnerableTime -= p_timeSinceLastFrame;
			mBlinkTime -= p_timeSinceLastFrame;
		}
		else
		{
			shipNode->setVisible(true);
			mInvulnerable = false;
		}
	}

	iceAnimationPtr->update(p_timeSinceLastFrame);
}

void icePlayer::playShotSound(){
	iceSoundManager* soundMgr = iceSoundManager::getSingletonPtr();
	if( mCurrentWeapon == MACHINEGUN ){
		soundMgr->PlaySound( 1, Ogre::Vector3::ZERO, 0 );
	}else if( mCurrentWeapon == SHOTGUN ){
		soundMgr->PlaySound( 2, Ogre::Vector3::ZERO, 0 );
	}else if( mCurrentWeapon == MISILE_LAUNCHER ){
		soundMgr->PlaySound( 3, Ogre::Vector3::ZERO, 0 );
	}
}

void icePlayer::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
	Ogre::Vector3 bPos =   shipNode->_getDerivedPosition() + ((cursorNode->_getDerivedPosition() - shipNode->_getDerivedPosition() ) / 16);
	iceBulletMgr::getSingletonPtr()->createBullet(true, "bt_player_",p_iWeapon, bPos, -shipNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
}

void icePlayer::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical)
{
	icePlayerStats::getInstance()->addReceivedDamage(p_iDamage);
	iceDamageTextManager::getSingletonPtr()->showPlayerDamage(icePhysicEntity::getGeometry()->getMovableObject(),p_iDamage,p_bCritical);
	iceGame::getUI()->getHUD()->setLife(getCurrentLife(),getMaxLife());

	if((Ogre::Real)p_iDamage/(Ogre::Real)getMaxLife() > 0.2f)
	{
		iceGame::getUI()->getHUD()->setFace(2); //Cara de daño recibido
	}

}

void icePlayer::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
	iceDamageTextManager::getSingletonPtr()->showPlayerShieldDamage(icePhysicEntity::getGeometry()->getMovableObject(),p_iDamage,p_bCritical);
}

void icePlayer::showHeal(unsigned int p_iHeal)
{
	iceDamageTextManager::getSingletonPtr()->showPlayerHeal(icePhysicEntity::getGeometry()->getMovableObject(),p_iHeal);
	iceGame::getUI()->getHUD()->setLife(getCurrentLife(),getMaxLife());
}

void icePlayer::showFail(void)
{
	icePlayerStats::getInstance()->addBulletEvaded();
	iceDamageTextManager::getSingletonPtr()->showPlayerMiss(icePhysicEntity::getGeometry()->getMovableObject());
}

void icePlayer::showLevelUp(unsigned int p_iLevel)
{
	iceDamageTextManager::getSingletonPtr()->showPlayerLevelUp(icePhysicEntity::getGeometry()->getMovableObject());
	iceGame::getUI()->getHUD()->setLevel(mLevel);
	iceGame::getUI()->getHUD()->setLife(getCurrentLife(),getMaxLife());
	icePlayerStats::getInstance()->setPlayerLevel(mLevel);
}

void icePlayer::setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel)
{
	iceRPG::setWeaponLevel(p_iWeapon, p_iLevel);
	icePlayerStats::getInstance()->setWeaponLevel(p_iWeapon, p_iLevel);
	iceGame::getUI()->getHUD()->setWeapon(mCurrentWeapon+1,mWeaponLevel[mCurrentWeapon]);
	if(p_iWeapon == MACHINEGUN)
	{
		iceDamageTextManager::getSingletonPtr()->showMinigunLevelUp(icePhysicEntity::getGeometry()->getMovableObject());
	}
	else if(p_iWeapon == SHOTGUN)
	{
		iceDamageTextManager::getSingletonPtr()->showShotgunLevelUp(icePhysicEntity::getGeometry()->getMovableObject());
	}
	else if(p_iWeapon == MISILE_LAUNCHER)
	{
		iceDamageTextManager::getSingletonPtr()->showMissileLevelUp(icePhysicEntity::getGeometry()->getMovableObject());
	}
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
		icePostProcessManager::getSingleton().enableHardBlur();
	}
}

void icePlayer::brake(void)
{
	if(mBrakeTime <= 0)
	{
		mBrakeTime = BRAKE_TIME;
		mSprintTime = 0; //deactivate sprint
		icePostProcessManager::getSingleton().disableBlur();
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
	Ogre::Real barrelDistance = getManiobrability() * BARREL_ROLL_TIME * BARREL_ROLL_DISTANCE_MULTIPLIER;
	Ogre::Real barrelRatio = (*pTime)/BARREL_ROLL_TIME;
	Ogre::Real ratioDifference = lastBarrelRatio - barrelRatio;

	Ogre::Vector3 translation = ratioDifference * pDirection * barrelDistance * Ogre::Vector3::UNIT_X;
	Ogre::Radian rotation = ratioDifference * (-pDirection) * Ogre::Degree(180) * 4;

	shipNode->translate(translation);
	rollNode->roll(rotation);
}

void icePlayer::setMovingUp(bool pMovingUp)
{
	if(mMovingUp) _velocityY = MAX_VELOCITY;

		mMovingUp = pMovingUp;
}

void icePlayer::setMovingDown(bool pMovingDown)
{
	if(mMovingDown) _velocityY = MAX_VELOCITY;

		mMovingDown = pMovingDown;
}

void icePlayer::setMovingLeft(bool pMovingLeft)
{
	if(mMovingLeft) _velocityX = MAX_VELOCITY;
		
	mMovingLeft = pMovingLeft;
}

void icePlayer::setMovingRight(bool pMovingRight)
{
	if(mMovingRight) _velocityX = MAX_VELOCITY;

	mMovingRight = pMovingRight;
}

bool icePlayer::isPositionBackToPlayer(Ogre::Vector3 pPosition)
{
	Ogre::Vector3 normal = mNode->_getDerivedOrientation() * Ogre::Vector3::UNIT_Z;
	Ogre::Vector3 playerPosition = mNode->_getDerivedPosition();

	return normal.dotProduct(playerPosition) >= normal.dotProduct(pPosition);
}

Ogre::Real icePlayer::getTimeMultiplier(void)
{
	Ogre::Real multiplier = 1;

	if(mSprintTime > 0)
	{
		multiplier = SPRINT_MULTIMPLICATOR;
	}	
	else if(mBrakeTime > 0)
	{
		multiplier = 1 / BRAKE_DIVISOR;
	}

	return multiplier;
}

void icePlayer::resetPositions(void)
{
	cursorNode->setPosition(Ogre::Vector3::ZERO);
	shipPlaneNode->setPosition(Ogre::Vector3::ZERO);
	shipNode->setPosition(Ogre::Vector3::ZERO);
}

bool icePlayer::isInvulnerable(void)
{
	return mInvulnerable;
}
void icePlayer::setInvulnerable(bool invunerable)
{
	mInvulnerable = invunerable;
	if(mInvulnerable)
	{
		mInvulnerableTime = INVULNERABLE_TIME;
	}
	else
	{
		mInvulnerableTime = 0;
	}
}
