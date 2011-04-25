#include "icePhase.h"
#include "iceStep.h"

#include <scene\DotSceneLoader.h>
#include <OgreQuaternion.h>
#include <stdlib.h>
#include <vector>

#define NUM_CAMS 7

icePhase::icePhase()
{
	
}

icePhase::~icePhase()
{
	//Deleting enemies
	for (unsigned int i=0;i<mEnemies.size();i++)
		delete mEnemies[i];
}

bool icePhase::initialize()
{
	return true;
}

void icePhase::finalize()
{

}

void icePhase::update(Ogre::Real p_timeSinceLastFrame)
{
	for (unsigned int i=0;i<mEnemies.size();i++)
		mEnemies[i]->update(p_timeSinceLastFrame);
}

bool icePhase::createScene( Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer )
{
	mSceneManager = p_SceneMgr;
	mPlayer = p_psPlayer;

	//Creating the CamVector
	stringstream strCamName;
	for( int i = 0; i < NUM_CAMS; i++ ){
		strCamName << "Camera " << i;
		// If this camera doesn't exist
		if( mSceneManager->hasCamera( strCamName.str() ) == false ){
			mvCameras.push_back( p_SceneMgr->createCamera( strCamName.str() ) );
		}else{
		// If this camera exists from other level
			mvCameras.push_back( p_SceneMgr->getCamera( strCamName.str() ) );
		}

		strCamName.clear();
	}

	// Setting camera parameters
	setCameras();
	return true;
}

Ogre::Camera* icePhase::getCamera( int p_iIndex ){
	return mvCameras[ p_iIndex ];
}
bool icePhase::clearScene(){
	std::stringstream strMessage;
	strMessage << "LEVEL: Clearing level";
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mLog->logMessage( strMessage.str() );

	mSceneManager->clearScene();

	strMessage.clear();
	strMessage << "LEVEL: Level cleared.";
	mLog->logMessage( strMessage.str() );

	return true;
}

vector<iceStep> icePhase::getStepsFromResources(ConfigNode* p_sStepsNode)
{
	vector<iceStep> steps;

	int stepsNumber = p_sStepsNode->findChild("stepsNumber")->getValueI();
	steps.resize(stepsNumber);

	for(int i=0;i<stepsNumber;i++)
	{
		char stepName[10];
		sprintf(stepName,"step%d",i+1);
		ConfigNode* stepNode = p_sStepsNode->findChild(stepName);

		Ogre::Real posX = stepNode->findChild("position")->getValueF(0);
		Ogre::Real posY = stepNode->findChild("position")->getValueF(1);
		Ogre::Real posZ = stepNode->findChild("position")->getValueF(2);

		Ogre::Radian rollAngle = Ogre::Degree(stepNode->findChild("rollAngle")->getValueF());
		Ogre::Real time = stepNode->findChild("time")->getValueF();
		steps[i] = iceStep(Ogre::Vector3(posX,posY,posZ),rollAngle,time);
	}

	return steps;
}

void icePhase::loadEnemies(ConfigNode* p_sEnemiesNode)
{
	vector<iceStep> steps;
	int enemiesNumber = p_sEnemiesNode->findChild("enemiesNumber")->getValueI();

	mEnemies.resize(enemiesNumber);
	for(int i=0;i<enemiesNumber;i++)
	{
		char enemyName[10];
		sprintf(enemyName,"enemy%d",i+1);
		ConfigNode* enemyNode = p_sEnemiesNode->findChild(enemyName);

		iceEnemy::ENEMYTYPE enemyType;

		switch(enemyNode->findChild("enemyType")->getValueI())
		{
			case 0:
				enemyType = iceEnemy::MINIMAGMATON;
				break;
			case 1:
				enemyType = iceEnemy::KAMIKAZE;
				break;
			case 2:
				enemyType = iceEnemy::INTELLIGENT;
				break;
			case 3:
				enemyType = iceEnemy::VOLCANO;
				break;
			case 4:
				enemyType = iceEnemy::MAGMATON;
				break;
			default:
				enemyType = iceEnemy::MINIMAGMATON;
				break;
		}
		Ogre::Real activationTime = enemyNode->findChild("activationTime")->getValueF();
		bool isAttachedToPlayer = enemyNode->findChild("isAttachedToPlayer")->getValueI();

		//Trajectory
		ConfigNode* trajectoryNode = enemyNode->findChild("trajectory");
		steps = getStepsFromResources(trajectoryNode);

		mEnemies[i] = new iceEnemy();
		mEnemies[i]->initialize(mSceneManager,mPlayer,activationTime,enemyType,isAttachedToPlayer);
		mEnemies[i]->setTrajectory(new iceTrajectory());
		mEnemies[i]->getTrajectory()->loadSteps(steps,true);
		mEnemies[i]->getTrajectory()->setNodeToLookAt(mPlayer->shipNode);
	}
}

vector<iceEnemy*>& icePhase::getEnemies(){
	return mEnemies;
}