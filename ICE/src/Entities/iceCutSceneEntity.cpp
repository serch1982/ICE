#include "Entities/iceCutSceneEntity.h"
#include "iceGame.h"

Ogre::NameGenerator iceCutSceneEntity::mNameGenerator("CutSceneEntity_");

iceCutSceneEntity::iceCutSceneEntity()
{
	mActivationTime = -1;
	mCurrentTime = 0;
}

iceCutSceneEntity::~iceCutSceneEntity()
{
	//switch(mEntityType)
	//{
	//	case SHIP:
	//		break;
	//	case MINI:
	//		break;
	//	case SMART:
	//		break;
	//	case KAMIKACE:
	//		break;
	//	case VOLCANO:
	//		break;
	//	case MAGMATON:
	//		break;
	//	case PIGSHEEP:
	//		break;
	//}
	iceAnimationPtr.reset();

}

bool iceCutSceneEntity::initialize(TYPE pEntityType, Ogre::Vector3 p_Position, Ogre::Real p_ActivationTime, iceTrajectory* pTrajectory)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mActivationTime = p_ActivationTime;
	mCurrentTime = 0;
	mEntityType = pEntityType;
	Ogre::String name = mNameGenerator.generate();
	mCutSceneEntityNode = sceneManager->getRootSceneNode()->createChildSceneNode(name);
	iceTrajectoryFollower::initialize(mCutSceneEntityNode);
	Ogre::Entity* mesh;
	Ogre::SceneNode* miniNode;
	Ogre::Vector3 scale;
	Ogre::Real arr[5];
	int ran;
	iceAnimationPtr = iceAnimationMgrPtr(new iceAnimationMgr());
	switch(mEntityType)
	{
		case SHIP:
			mesh = sceneManager->createEntity(name + "_airplane", "airplane.mesh");

			mesh->attachObjectToBone("turbo_right",sceneManager->createEntity(name + "_jet1", "plane_jet.mesh"),Ogre::Vector3::UNIT_Z.getRotationTo(Ogre::Vector3::NEGATIVE_UNIT_Z),Ogre::Vector3(0.2,0,0.2));
			mesh->attachObjectToBone("turbo_left",sceneManager->createEntity(name + "_jet2", "plane_jet.mesh"),Ogre::Quaternion::IDENTITY,Ogre::Vector3(0.2,0,-0.2));

			//init animations
			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("iddle2_Clip"));
			iceAnimationPtr->addAnimation(mesh->getAnimationState("enfado2_Clip"));
			iceAnimationPtr->addAnimation(mesh->getAnimationState("celebracion2_Clip"),0,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("impacto4_Clip"),0.5,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("giro_izquierda1_Clip"));
			iceAnimationPtr->addAnimation(mesh->getAnimationState("giro_derecha1_Clip"));
			break;
		case MINI:
			mesh = sceneManager->createEntity("minimagmaton.mesh");

			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("iddle_Clip1"));
			iceAnimationPtr->addAnimation(mesh->getAnimationState("attack_Clip"),1,false,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("flying01_Clip"),0,false,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("flying02_Clip"),0,false,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("flying03_Clip"),0,false,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("flying04_Clip"),0,false,true);
			iceAnimationPtr->addAnimation(mesh->getAnimationState("hit01_Clip"),0,true); //dead animation too
			iceAnimationPtr->addAnimation(mesh->getAnimationState("hit02_Clip"),0,true);

			partice1 = iceParticleMgr::getSingletonPtr()->createPartAttachToBone(mesh,"column","ice/fireDown",false);

			break;
		case SMART:
			mesh = sceneManager->createEntity("minimagmaton.mesh");
			miniNode = mNode->createChildSceneNode(Ogre::Vector3(0,1,0));
			miniNode->attachObject(mesh);

			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("attack_Clip"));
			mesh = sceneManager->createEntity("intelligent.mesh");
			mNode->attachObject(mesh);
			break;
		case KAMIKACE:
			mesh = sceneManager->createEntity("kamikace.mesh");
			arr[0] = 0.3;
			arr[1] = 0.6;
			arr[2] = 1.0;
			arr[3] = 2.0;
			ran = rand() % 4;
			scale = Ogre::Vector3(Ogre::Vector3::UNIT_SCALE * arr[ran] );
			mNode->setScale(scale);

			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("fly"));

			partice1 = iceParticleMgr::getSingletonPtr()->createPartAttachToObject(mNode,"ice/iceKamimaze",false);

			break;
		case VOLCANO:
			mesh = sceneManager->createEntity("volcano.mesh");
			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("iddle")); 
			iceAnimationPtr->addAnimation(mesh->getAnimationState("attack"),0, true); 
			break;
		case MAGMATON:
			mesh = sceneManager->createEntity("magmaton.mesh");
			break;
		case PIGSHEEP:
			mesh = sceneManager->createEntity("pigsheep.mesh");
			iceAnimationPtr->setIddleAnimation(mesh->getAnimationState("iddle"));
			break;
	}

	mNode->attachObject(mesh);
	mNode->setPosition(p_Position);

	if(pTrajectory)
	{
		setTrajectory(pTrajectory);
	}
	else
	{
		//Dummy Trajectory
		setTrajectory(new iceTrajectory());
	}

	deactivate();

	return true;
}

void iceCutSceneEntity::finalize()
{
	switch(mEntityType)
	{
		case SHIP:
			break;
		case MINI:
			iceParticleMgr::getSingletonPtr()->removeParticle(partice1);		
			break;
		case SMART:
			break;
		case KAMIKACE:
			iceParticleMgr::getSingletonPtr()->removeParticle(partice1);
			break;
		case VOLCANO:
			break;
		case MAGMATON:
			break;
		case PIGSHEEP:
			break;
	}
}

void iceCutSceneEntity::update(Ogre::Real p_TimeSinceLastFrame)
{
	if(mIsActive)
	{
		iceTrajectoryFollower::update(p_TimeSinceLastFrame);
		iceAnimationPtr->update(p_TimeSinceLastFrame);
		//mTrajectory->lookAt();
		if(mTrajectory->hasEnded())
			deactivate();
	}
	else
	{
			if(checkActivationTime(p_TimeSinceLastFrame))
			{//active
				activate();
			}
	}
}

Ogre::Real iceCutSceneEntity::getActivationTime(void)
{
	return mActivationTime;
}

void iceCutSceneEntity::setActivationTime(Ogre::Real p_ActivationTime)
{
	mActivationTime = p_ActivationTime;
}

bool iceCutSceneEntity::isActive(void)
{
	return mIsActive;
}

void iceCutSceneEntity::activate(void)
{
	mNode->setVisible(true);
	mIsActive = true;
}

void iceCutSceneEntity::deactivate(void)
{
	mNode->setVisible(false);
	mIsActive = false;
}

bool iceCutSceneEntity::checkActivationTime(Ogre::Real p_timeSinceLastFrame)
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

void iceCutSceneEntity::startAnimation(Ogre::String name)
{
	iceAnimationPtr->startAnimation(name);
}

void iceCutSceneEntity::stopAnimations()
{
	iceAnimationPtr->stopAllAnimations();
}

bool iceCutSceneEntity::hasAnimationEnded(Ogre::String name)
{
	return iceAnimationPtr->hasAnimationEnded(name);
}
