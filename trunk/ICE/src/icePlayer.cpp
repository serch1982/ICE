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

	//init camera
	cameraPlaneNode = playerNode->createChildSceneNode(Ogre::Vector3(0.0,0.0,-500.0));
	cameraNode = cameraPlaneNode->createChildSceneNode(Ogre::Vector3(0.0,100.0,0.0));

	return true;
}

void icePlayer::setCamera(Ogre::Camera* camera)
{
	cameraNode->removeAllChildren();
	playerCamera = camera;
	cameraNode->attachObject(camera);
	camera->lookAt(cursorNode->convertLocalToWorldPosition(cursorNode->getPosition()));
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

void icePlayer::update()
{

}
