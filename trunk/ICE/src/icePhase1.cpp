#include "icePhase1.h"


#define NUM_CAMS 7

icePhase1::icePhase1()
{
	
}

icePhase1::~icePhase1()
{
	icePhase::~icePhase();
}

void icePhase1::update( Ogre::Real p_timeSinceLastFrame)
{
	//call logic layer to change the state of the enemy
	iceLogicLua::getInstance()->getEnemyLogicState(&mEnemy);
	mEnemy.update(p_timeSinceLastFrame);
}

bool icePhase1::createScene( Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer )
{
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mLog->logMessage( "LEVEL: Loading Phase 1" );

	icePhase::createScene(p_SceneMgr,p_psPlayer);

	int j = 0;
	vector<iceStep> steps;

	//ConfigNode *rootNode;
	//rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "Crate");

	steps.push_back(iceStep(Ogre::Vector3( -907, 535 , -667),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -739, 407 , -713),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -483, 252 , -559),Ogre::Degree(0),5*j++));
	steps.push_back(iceStep(Ogre::Vector3( -326, 120 , -289),Ogre::Degree(0),5*j++));
		steps.push_back(iceStep(Ogre::Vector3( -326, 119 , -289),Ogre::Degree(0),5*j++));


	//steps.push_back(iceStep(Ogre::Vector3( -186,  48 ,  -6 ),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3( -4  ,  54 , 237 ),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(  8  ,  52 , 511 ),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(  7  , 90  , 706 ),Ogre::Degree(-0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3( -60 , 171 ,993  ),Ogre::Degree(-35),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(  100, 238 ,1274 ),Ogre::Degree(-40),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3( 314 , 121 ,966  ),Ogre::Degree(-35),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(302  , 71  , 95  ),Ogre::Degree(-20),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3( 440 , 68  , -107),Ogre::Degree(0),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(1526 , 95  , 102 ),Ogre::Degree(40),5*j++));
	//steps.push_back(iceStep(Ogre::Vector3(1527 , 95  , 102 ),Ogre::Degree(0),5*j++));


	//steps.push_back(iceStep(Ogre::Vector3( 500, 500 , -500),Ogre::Degree(0),10*j++));
	//steps.push_back(iceStep(Ogre::Vector3( 500, 500 , -700),Ogre::Degree(0),10*j++));
	//steps.push_back(iceStep(Ogre::Vector3( 500, 500 , -900),Ogre::Degree(0),10*j++));


	mPlayer->setTrajectory(new iceLocomotiveTrajectory());

	mPlayer->getTrajectory()->loadSteps(steps,false);

	
	j = 0;
	steps.clear();

	//ConfigNode *rootNode;
	//rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "Crate");

	steps.push_back(iceStep(Ogre::Vector3( 30, 0 , 200),Ogre::Degree(0),j++));
	steps.push_back(iceStep(Ogre::Vector3( 0, 30 , 230),Ogre::Degree(0),j++));
	steps.push_back(iceStep(Ogre::Vector3( -30, 0 , 200),Ogre::Degree(0),j++));
	steps.push_back(iceStep(Ogre::Vector3( 0, -30 , 170),Ogre::Degree(0),j++));
	steps.push_back(iceStep(Ogre::Vector3( 30, 0 , 200),Ogre::Degree(0),j++));

	mEnemy.initialize(mSceneManager,mPlayer);
	mEnemy.setTrajectory(new iceTrajectory());
	mEnemy.getTrajectory()->loadSteps(steps,true);
	mEnemy.getTrajectory()->setNodeToLookAt(mPlayer->shipNode);
	mEnemy.activate();

	// Creating a DotSceneLoader
	DotSceneLoader pLoader;
	// Loading a Level. Level1 by default.
	pLoader.parseDotScene( "level1.scene","level1",p_SceneMgr );

	mSceneManager->setSkyBox( true, "cielo", 20000.0f, false, Ogre::Quaternion::IDENTITY, "level1" ); 

	// Set ambient light
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneManager->createLight("MainLight");
	l->setType( Ogre::Light::LT_DIRECTIONAL);
	l->setDirection( -1, 1, -1 );
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(1.0, 1.0, 1.0);

	mLog->logMessage( "LEVEL: Phase 1 loaded." );
	
	return true;
}

void icePhase1::setCameras(){

	//CAM 0
	mvCameras[0]->setPosition(Ogre::Vector3(-260,650,590));
	mvCameras[0]->lookAt(Ogre::Vector3(-260,0,590));
	mvCameras[0]->setNearClipDistance(5);
	
	//CAM 1
	mvCameras[1]->setPosition(Ogre::Vector3(-400,10,-500));
	mvCameras[1]->lookAt(Ogre::Vector3(-200,10,-500));
	mvCameras[1]->setNearClipDistance(5);
	
	//CAM 2
	mvCameras[2]->setPosition(Ogre::Vector3(0,1000,0));
	mvCameras[2]->lookAt(Ogre::Vector3(0,0,0));
	mvCameras[2]->setNearClipDistance(5);
	mvCameras[2]->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	
	//CAM 3
	mvCameras[3]->setPosition(Ogre::Vector3(-880,5,612));
	mvCameras[3]->lookAt(Ogre::Vector3(-227,10,880));
	mvCameras[3]->setNearClipDistance(5);

	//CAM 4
	mvCameras[4]->setPosition(Ogre::Vector3(492,10,462));
	mvCameras[4]->lookAt(Ogre::Vector3(-161,10,218));
	mvCameras[4]->setNearClipDistance(5);

	//CAM 5
	mvCameras[5]->setPosition(Ogre::Vector3(-168,10,684));
	mvCameras[5]->lookAt(Ogre::Vector3(-142,10,520));
	mvCameras[5]->setNearClipDistance(5);
	
	//CAM 6
	mvCameras[6]->setPosition(Ogre::Vector3(302,10,-115));
	mvCameras[6]->lookAt(Ogre::Vector3(302,10,-100));
	mvCameras[6]->setNearClipDistance(5);

}

bool icePhase1::isPhaseEnded(void)
{
	return mPlayer->getTrajectory()->hasEnded();
}