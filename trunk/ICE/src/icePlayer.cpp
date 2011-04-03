#include <icePlayer.h>

icePlayer::icePlayer()
{

}

icePlayer::~icePlayer(){

}

bool icePlayer::initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node)
{
	playerNode = node;
	shipMaxVelocity = 200;

	//Init cursor
	cursorPlaneNode = playerNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,500.0));	
	cursorNode = cursorPlaneNode->createChildSceneNode();
	
	Ogre::Entity* mesh = sceneManager->createEntity("cursorMesh", "sphere.mesh");
	cursorNode->attachObject(mesh);
	cursorNode->scale(0.1,0.1,0.1);

	//Init Ship
	shipPlaneNode = playerNode->createChildSceneNode();
	shipNode = shipPlaneNode->createChildSceneNode();

	Ogre::Entity* mesh2 = sceneManager->createEntity("shipMesh", "nave.mesh");
	mesh2->setCastShadows(true);
	shipNode->attachObject(mesh2);
	//shipNode->scale(0.2,0.2,0.2);

	//Ogre::ParticleSystem* smoke1 = sceneManager->createParticleSystem("Smoke1", "Smoke2");
	//Ogre::ParticleSystem* smoke2 = sceneManager->createParticleSystem("Smoke2", "Smoke2");
	//Ogre::SceneNode* smokeNode1 = node->createChildSceneNode(Ogre::Vector3(5.0,10.0,-70.0));
	//smokeNode1->attachObject(smoke1);
	//Ogre::SceneNode* smokeNode2 = node->createChildSceneNode(Ogre::Vector3(-17.0,10.0,-70.0));
	//smokeNode2->attachObject(smoke2);

	// Init camera
	cameraPlaneNode = playerNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-500.0));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,/*10*/0.0,0.0));

	if( !sceneManager->hasCamera("PlayerCam") )
		setCamera( sceneManager->createCamera( "PlayerCam" ) );

	// Initialize bullets
	
	mainBulletNode = sceneManager->getRootSceneNode()->createChildSceneNode("bulletMainNode",Ogre::Vector3( 0, 0, 0 ));		
	//iceBullet b;

	/*Create Machineguns*/
	int i = 0;
	//for(i = 1; i < MAX_BULLETS_PER_WEAPON + 1; i++)
	for(i = 0; i < MAX_BULLETS_PER_WEAPON; i++)
	{		
		mvBullets[i].CreateEntities(sceneManager,mainBulletNode,MACHINEGUN);				
		//b.CreateEntities(sceneManager,mainBulletNode,MACHINEGUN);				
		//mvBullets.push_back(b);	
	}	
	/*Create Shotguns*/
	//for (i = 1; i < MAX_BULLETS_PER_WEAPON + 1; i++)
	for(i = MAX_BULLETS_PER_WEAPON; i < 2*MAX_BULLETS_PER_WEAPON; i++)
	{		
		mvBullets[i].CreateEntities(sceneManager,mainBulletNode,SHOTGUN);				
		//b.CreateEntities(sceneManager,mainBulletNode,SHOTGUN);				
		//mvBullets.push_back(b);	
	}
	/*Create MisileLaunchers*/
	//for (i = 1; i < MAX_BULLETS_PER_WEAPON + 1 ; i++)
	for(i = 2*MAX_BULLETS_PER_WEAPON; i < 3*MAX_BULLETS_PER_WEAPON; i++)
	{		
		mvBullets[i].CreateEntities(sceneManager,mainBulletNode,MISILE_LAUNCHER);				
		//b.CreateEntities(sceneManager,mainBulletNode,MISILE_LAUNCHER);				
		//mvBullets.push_back(b);	
	}


	return true;
}

void icePlayer::setCamera(Ogre::Camera* camera)
{
	cameraNode->removeAllChildren();
	playerCamera = camera;
	cameraNode->attachObject(camera);
	camera->lookAt(cursorNode->convertLocalToWorldPosition(cursorNode->getPosition()));
}

Ogre::Camera* icePlayer::getCamera(){
	return playerCamera;
}

void icePlayer::processMouseMoved(const OIS::MouseEvent &arg)
{
	cursorNode->translate((Ogre::Real)-arg.state.X.rel,(Ogre::Real)-arg.state.Y.rel,0);
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
	
	/* Update active bullets*/
	int i=0;
	//for(i = 0; i<mvBullets.size();i++)
	for(i=0;i<30-1;i++)
	{
		mvBullets[i].Update(p_timeSinceLastFrame);	
	}
}

void icePlayer::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic)
{
	/* Activate the first free bullet*/
	int i = 0;
	bool bFreeBullet = false;
	while(!bFreeBullet)
	{
		if(mvBullets[i].Set(shipNode,0,false))
		{
			bFreeBullet = true;
			
		}else
		{
			//if(i<mvBullets.size())
			if (i<30)
			{
				i++;
			}
		}
	}	
}

void icePlayer::showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical)
{
}

void icePlayer::showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical)
{
}

void icePlayer::showFail(void)
{
}

void icePlayer::showLevelUp(void)
{
}
