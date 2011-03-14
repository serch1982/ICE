/*
-----------------------------------------------------------------------------
Filename:    ICE.cpp
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (August 2010)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/

#include "ICE.h"

//-------------------------------------------------------------------------------------
ICE::ICE(void)
{
}
//-------------------------------------------------------------------------------------
ICE::~ICE(void)
{
}

//-------------------------------------------------------------------------------------
void ICE::createScene(void)
{
	mLevel.createScene( mSceneMgr );

	// Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType( Ogre::Light::LT_DIRECTIONAL);
	l->setDirection( -1, 1, -1 );
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(1.0, 1.0, 1.0);

    /*Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead);

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);*/


	int jur = 1000;
	int j = 0;
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(0,300,0),Ogre::Degree(0),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,300,0),Ogre::Degree(20),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,500,jur*2),Ogre::Degree(-20),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,500,0),Ogre::Degree(0),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,300,-jur*2),Ogre::Degree(0),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,500,jur*2),Ogre::Degree(-20),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,500,0),Ogre::Degree(0),5*j++));
	mTrajectory.addStep(iceTrajectoryStep(Ogre::Vector3(jur*j,300,-jur*2),Ogre::Degree(0),5*j++));

	//Putting elements in the scene
	//icePlayer play (mSceneMgr,mSceneMgr->getRootSceneNode());
	//player = &play;
	mPlayer.initialize(mSceneMgr,mSceneMgr->getRootSceneNode()->createChildSceneNode());
	mPlayer.setCamera(mCameras[1]);
	mTrajectory.loadSteps(mSceneMgr,mPlayer.playerNode);
}

bool ICE::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
 
	
	mTrajectory.addTime(evt.timeSinceLastFrame);
	if (mICEMenu->instance()->getState() != ICEMenu::GOD)
		mPlayer.updateShipPosition(evt.timeSinceLastFrame);

    return ret;
}

bool ICE::mouseMoved( const OIS::MouseEvent &arg )
{
	bool ret = BaseApplication::mouseMoved(arg);
	mPlayer.processMouseMoved(arg);
    return ret;
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ICE app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
