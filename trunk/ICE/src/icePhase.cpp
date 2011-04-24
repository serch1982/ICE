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

vector<iceEnemy*>& icePhase::getEnemies(){
	return mEnemies;
}