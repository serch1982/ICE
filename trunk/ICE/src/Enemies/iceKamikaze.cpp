#include "Enemies\iceKamikaze.h"
#include "iceGame.h"

iceKamikaze::iceKamikaze(){
	iceEnemy::iceEnemy();
	mRenewTarget = 100;
}

iceKamikaze::~iceKamikaze(){}

bool iceKamikaze::initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer){
	if( !iceEnemy::initialize( id, p_Position, p_psPlayer, p_fActivationTime, p_isAttachedToPlayer ) )
		return false;
	
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();

	//creating a name
	std::stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();

	// loading the mesh and attaching it to he node
	Ogre::Entity* mesh;
	mesh = sceneManager->createEntity(entityName.str(), "kamikaze.mesh");
	mNode->attachObject(mesh);

	//init physics mesh
	icePhysicEntity::initialize(mesh);

	return true;
}

void iceKamikaze::finalize(){
	iceEnemy::finalize();
}

void iceKamikaze::update(Ogre::Real p_timeSinceLastFrame){
	switch(mState)
	{
		case STOPPED:
			mTrajectory->lookAt();
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case FOLLOWING_TRAJECTORY:
			//iceTrajectoryFollower::update(p_timeSinceLastFrame); Hay que hablar sobre trayectorias de enemigos
			mTrajectory->lookAt();//TODO
			/*if (!isAlive())
				mState = DYING;*/
			break;
		case ATTACK: 
			mTrajectory->lookAt(); //TODO
			enemyNode->translate( mVelocity * p_timeSinceLastFrame );
			mRenewTarget--;
			if( mRenewTarget == 0 ){
				/*Ogre::Vector3 playerPos;
				Ogre::Vector3 kamikazePos;
				//Get Player position relative to his parent
				playerPos = mPlayer->shipNode->getPosition();
				//Convert playerPos to World Coordinates
				mTargetPosition = mPlayer->shipNode->convertLocalToWorldPosition(playerPos);
				//Get Kamikaze WORLD position
				kamikazePos = enemyNode->convertLocalToWorldPosition( kamikazePos );
				//mVelocity = (mTargetPosition-kamikazePos) / 0.7; //Provisional
				mRenewTarget = 100;*/
			}
			break;
		case DYING:
			//to update billboard
			iceGame::getGameLog()->logMessage("Enemy killed! DYING");
			mAnimDyingTicks++;
			break;
		case INACTIVE:
			if(checkActivationTime(p_timeSinceLastFrame))
			{//active
				activate();
			}
			else
			{//inactive
				mNode->setVisible(false);
				if(mShowingBoundingBox)
				{
					icePhysicEntity::hideBoundingBox();
				}
			}
			break;
	}
	mBillboard->update(p_timeSinceLastFrame);
}

std::string iceKamikaze::getFunctionStr(){
	return "KamikazeLogic";
}

void iceKamikaze::setState(ENEMYSTATE p_iState){
	mState = p_iState;
	Ogre::Vector3 playerPos;
	Ogre::Vector3 kamikazePos(0.0f,0.0f,0.0f);
	std::stringstream msg;
	switch(mState){
		case ATTACK:
			//Get Player position relative to his parent
			playerPos = mPlayer->shipNode->getPosition();
			//Convert playerPos to World Coordinates
			mTargetPosition = mPlayer->shipNode->convertLocalToWorldPosition(playerPos);
			//Get Kamikaze WORLD position
			kamikazePos = enemyNode->convertLocalToWorldPosition( kamikazePos );
			mVelocity = (mTargetPosition-kamikazePos) / 0.7; //Provisional
			mRenewTarget = 100;
			//LOG
			msg << "PlayerPos:      X: "<< playerPos.x << "\tY: " << playerPos.y << "\tZ: " << playerPos.z << std::endl;
			msg << "mTargetPostion: X: " << mTargetPosition.x << "\tY: " << mTargetPosition.y << "\tZ: " << mTargetPosition.z << std::endl;
			msg << "kamikazePos:    X: " << kamikazePos.x << "\tY: " << kamikazePos.y << "\tZ: " << kamikazePos.z << std::endl;
			msg << "Velocity:       X: " << mVelocity.x << "\tY: " << mVelocity.y << "\tZ: " << mVelocity.z << std::endl;
			mLog->logMessage( msg.str() );
			break;
		case DYING:
			mAnimDyingTicks = 0;
			mBillboard->start(enemyNode->_getDerivedPosition());
			break;
		case DEAD:
			mPlayer->addExperience(mLevel * 10000);
		default:
			break;
	}
}
