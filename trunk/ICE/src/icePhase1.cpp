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
	iceLogicLua::getInstance()->getEnemyLogicState(mEnemies[0],p_timeSinceLastFrame );
	icePhase::update(p_timeSinceLastFrame);
}

bool icePhase1::createScene( Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer )
{
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mLog->logMessage( "LEVEL: Loading Phase 1" );

	icePhase::createScene(p_SceneMgr,p_psPlayer);

	//Loading player trajectory
	ConfigNode *rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "phase1Trajectory");
	vector<iceStep> steps = getStepsFromResources(rootNode);
	mPlayer->setTrajectory(new iceLocomotiveTrajectory());
	mPlayer->getTrajectory()->loadSteps(steps,false);

	//Loading enemies
	rootNode = ConfigScriptLoader::getSingleton().getConfigScript("entity", "phase1Enemies");
	loadEnemies(rootNode);

	Ogre::SceneNode* level = p_SceneMgr->getRootSceneNode()->createChildSceneNode( "level1" );


	// Creating a DotSceneLoader
	DotSceneLoader pLoader;
	// Loading a Level. Level1 by default.
	pLoader.parseDotScene( "level1.scene","level1",p_SceneMgr,level,"level1_" );

	mSceneManager->setSkyBox( true, "cielo", 20000.0f, true, Ogre::Quaternion::IDENTITY, "level1" ); 

	// Set ambient light
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneManager->createLight("MainLight");
	l->setType( Ogre::Light::LT_DIRECTIONAL);
	l->setDirection( -1, 1, -1 );
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(1.0, 1.0, 1.0);

	mLog->logMessage( "LEVEL: Phase 1 loaded." );

	// Pau - Create an advertisement panel with texture animation FX ------------------------------------------------------------------//

	Ogre::SceneNode* advPan1Node = mSceneManager->getRootSceneNode()->createChildSceneNode("advPan1Node",Ogre::Vector3( 240, 98, 290 ));		
	Ogre::Entity* advPan1 = mSceneManager->createEntity("cube.mesh");			
	advPan1Node->scale(3,3,.009);
	advPan1Node->attachObject(advPan1);

	Ogre::SceneNode* advPan1BillNode = advPan1Node->createChildSceneNode("advPan1BillboardNode",Ogre::Vector3( 0, 0, 0 ));		
	Ogre::BillboardSet* advPan1Set = mSceneManager->createBillboardSet();
	advPan1Set->setMaterialName("advertisementPanel1");
	advPan1Set->setBillboardType(Ogre::BillboardType::BBT_PERPENDICULAR_COMMON);
	Ogre::Billboard* advPan1Billboard = advPan1Set->createBillboard(0,0,0);
	advPan1BillNode->scale(.18,.18,.18);
	advPan1BillNode->translate(0,0,200,Ogre::Node::TS_LOCAL);
	advPan1BillNode->attachObject(advPan1Set);

	//---------------------------------------------------------------------------------------------------------------------------------//
	
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