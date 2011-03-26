#include "iceLevel.h"
#include "iceStep.h"

#include <scene\DotSceneLoader.h>
#include <OgreQuaternion.h>
#include <stdlib.h>
#include <vector>

iceLevel::iceLevel(){

}

iceLevel::~iceLevel(){

}

bool iceLevel::initialize(){
	return true;
}

void iceLevel::finalize(){

}

void iceLevel::update( Ogre::Real p_timeSinceLastFrame){
	mTrajectory.addTime( p_timeSinceLastFrame);
}

bool iceLevel::createScene( Ogre::SceneManager* p_SceneMgr, int m_iCurrentLevel ){
	std::stringstream strMessage;
	strMessage << "LEVEL: Loading Level " << m_iCurrentLevel;
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mLog->logMessage( strMessage.str() );

	int jur = 1000;
	int j = 0;
	vector<iceStep> steps;
	//steps.push_back(iceStep(Ogre::Vector3(0,300,0),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,300,0),Ogre::Degree(20),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,500,jur*2),Ogre::Degree(-20),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,500,0),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,300,-jur*2),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,500,jur*2),Ogre::Degree(-20),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,500,0),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(jur*j,300,-jur*2),Ogre::Degree(0),5*j++));

	steps.push_back(iceStep(Ogre::Vector3( -907, 535 , -667),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -739, 407 , -713),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -483, 252 , -559),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -326, 120 , -289),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -186,  48 ,  -6 ),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -4  ,  54 , 237 ),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(  8  ,  52 , 511 ),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(  7  , 90  , 706 ),Ogre::Degree(-20),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -60 , 171 ,993  ),Ogre::Degree(-40),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(  100, 238 ,1274 ),Ogre::Degree(-40),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( 314 , 121 ,966  ),Ogre::Degree(-20),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(302  , 71  , 95  ),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( 440 , 68  , -107),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(1526 , 95  , 102 ),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3(1527 , 95  , 102 ),Ogre::Degree(0),5*j++));

	mTrajectory.init( p_SceneMgr, p_SceneMgr->getSceneNode("icePlayer") );
	mTrajectory.loadSteps(steps);

	// Creating a DotSceneLoader
	DotSceneLoader pLoader;
	// Loading a Level. Level1 by default.
	pLoader.parseDotScene( "level1.scene","level1",p_SceneMgr );

	p_SceneMgr->setSkyBox( true, "cielo", 20000.0f, false, Ogre::Quaternion::IDENTITY, "level1" ); 

	// Set ambient light
    p_SceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = p_SceneMgr->createLight("MainLight");
	l->setType( Ogre::Light::LT_DIRECTIONAL);
	l->setDirection( -1, 1, -1 );
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(1.0, 1.0, 1.0);

	strMessage.clear();
	strMessage << "LEVEL: Level " << m_iCurrentLevel << " loaded.";
	mLog->logMessage( strMessage.str() );
	
	return true;
}

/*bool iceLevel::clearScene( Ogre::SceneManager* p_SceneMgr ){
	std::stringstream strMessage;
	strMessage << "LEVEL: Clearing level";
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mLog->logMessage( strMessage.str() );

	p_SceneMgr->clearScene();

	strMessage.clear();
	strMessage << "LEVEL: Level cleared.";
	mLog->logMessage( strMessage.str() );

	return true;
}*/