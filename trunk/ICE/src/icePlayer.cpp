#include <icePlayer.h>

icePlayer::icePlayer()
{

}

icePlayer::~icePlayer(){

}

bool icePlayer::initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node)
{
	playerNode = node;
	shipMaxVelocity = 20;

	//Init cursor
	cursorPlaneNode = playerNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,400.0));	
	cursorNode = cursorPlaneNode->createChildSceneNode();
	
	Ogre::Entity* mesh = sceneManager->createEntity("cursorMesh", "sphere.mesh");
	cursorNode->attachObject(mesh);
	cursorNode->scale(0.01,0.01,0.01);

	//Init Ship
	shipPlaneNode = playerNode->createChildSceneNode();
	shipNode = shipPlaneNode->createChildSceneNode();

	Ogre::Entity* mesh2 = sceneManager->createEntity("shipMesh", "nave.mesh");
	mesh2->setCastShadows(true);
	shipNode->attachObject(mesh2);
	shipNode->scale(0.04,0.09,0.09);

	//Ogre::ParticleSystem* smoke1 = sceneManager->createParticleSystem("Smoke1", "Smoke2");
	//Ogre::ParticleSystem* smoke2 = sceneManager->createParticleSystem("Smoke2", "Smoke2");
	//Ogre::SceneNode* smokeNode1 = node->createChildSceneNode(Ogre::Vector3(5.0,10.0,-70.0));
	//smokeNode1->attachObject(smoke1);
	//Ogre::SceneNode* smokeNode2 = node->createChildSceneNode(Ogre::Vector3(-17.0,10.0,-70.0));
	//smokeNode2->attachObject(smoke2);

	// Init camera
	cameraPlaneNode = playerNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-300.0));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,/*10*/0.0,0.0));

	if( !sceneManager->hasCamera("PlayerCam") )
		setCamera( sceneManager->createCamera( "PlayerCam" ) );


	//Pau * INITIALIZE BULLETS *----------------------------------------------------------------------------------------//
	
	mainBulletNode = sceneManager->getRootSceneNode()->createChildSceneNode("bulletMainNode",Ogre::Vector3( 0, 0, 0 ));		
	
	/*Create Machineguns*/
	int i = 0;
	
	for(i = 0; i < MACHINEGUN_RELOAD; i++)
	{		
		mvMachinegunBullets[i].CreateEntities(sceneManager,mainBulletNode,MACHINEGUN);
		iceDebugScreen::instance()->updateChivato(9,Ogre::StringConverter::toString(mMachinegunAmmo));
	}	
	
	/*Create Shotguns*/	
	for(i = 0; i < SHOTGUN_RELOAD; i++)
	{		
		mvShotgunBullets[i].CreateEntities(sceneManager,mainBulletNode,SHOTGUN);
		iceDebugScreen::instance()->updateChivato(10,Ogre::StringConverter::toString(mShotgunAmmo));
	}
	
	/*Create MisileLaunchers*/	
	for(i = 0; i < MISILE_LAUNCHER_RELOAD; i++)
	{
		mvMisilLauncherBullets[i].CreateEntities(sceneManager,mainBulletNode,MISILE_LAUNCHER);
		iceDebugScreen::instance()->updateChivato(11,Ogre::StringConverter::toString(mMisileLauncherAmmo));
	}

	iceCounters::instance();
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
	camera->setFOVy(Ogre::Degree(10));
}

Ogre::Camera* icePlayer::getCamera(){
	return playerCamera;
}

void icePlayer::processMouseMoved(const OIS::MouseEvent &arg)
{
	cursorNode->translate(((Ogre::Real)-arg.state.X.rel)/30,((Ogre::Real)-arg.state.Y.rel)/30,0);
}

void icePlayer::updateShipPosition(Ogre::Real frameTime)
{
	//updatePosition
	Ogre::Vector3 targetPosition = cursorNode->getPosition();
	Ogre::Real maxMovement = shipMaxVelocity * frameTime;
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

void icePlayer::finalize()
{

}

void icePlayer::update(Ogre::Real p_timeSinceLastFrame)
{
	updateShipPosition(p_timeSinceLastFrame);
	addExperience(1000);
	
	//Pau * UPDATE ACTIVE BULLETS *-----------------------------------------------------------------------//
	
	int i=0;
	
	for(i = 0; i < MACHINEGUN_RELOAD; i++)
	{
		mvMachinegunBullets[i].Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < SHOTGUN_RELOAD; i++)
	{
		mvShotgunBullets[i].Update(p_timeSinceLastFrame);
	}
	for(i = 0; i < MISILE_LAUNCHER_RELOAD; i++)
	{
		mvMisilLauncherBullets[i].Update(p_timeSinceLastFrame);
	}
	//-------------------------------------------------------------------------------------------------//
}

void icePlayer::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic)
{
	//Pau * ACTIVATE THE FIRST FREE BULLET OF THE CURRENT WEAPON*-------------------------------------------------------------//
	
	int i = 0;
	bool bFreeBulletFound = false;
	
	switch(p_iWeapon)
	{
		case MACHINEGUN:
		
			while((!bFreeBulletFound)&&(mMachinegunAmmo>0))
			{
				if(mvMachinegunBullets[i].Set(shipNode,p_iDamage,p_bCritic))
				{
					bFreeBulletFound = true;
					mMachinegunAmmo --;
					iceDebugScreen::instance()->updateChivato(9,Ogre::StringConverter::toString(mMachinegunAmmo));
				}else
				{		
					if (i<MACHINEGUN_RELOAD)
					{
						i++;
					}
				}
			}
			break;
	
		case SHOTGUN:
		
			while(!bFreeBulletFound)
			{
				if(mvShotgunBullets[i].Set(shipNode,p_iDamage,p_bCritic))
				{
					bFreeBulletFound = true;
					mShotgunAmmo--;
					iceDebugScreen::instance()->updateChivato(10,Ogre::StringConverter::toString(mShotgunAmmo));
				}else
				{		
					if (i<SHOTGUN_RELOAD)
					{
						i++;
					}
				}
			}
			break;

		case MISILE_LAUNCHER:
		
			while(!bFreeBulletFound)
			{
				if(mvMisilLauncherBullets[i].Set(shipNode,p_iDamage,p_bCritic))
				{
					bFreeBulletFound = true;
					mMisileLauncherAmmo --;
					iceDebugScreen::instance()->updateChivato(11,Ogre::StringConverter::toString(mMisileLauncherAmmo));
				}else
				{		
					if (i<MISILE_LAUNCHER_RELOAD)
					{
						i++;
					}
				}
			}
			break;
	}
	//-------------------------------------------------------------------------------------------------//

}

void icePlayer::showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical)
{
	iceCounters::instance()->addReceivedDamage(p_iDamage);
}

void icePlayer::showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical)
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
