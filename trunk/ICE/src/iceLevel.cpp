#include <iceLevel.h>

#include <scene\DotSceneLoader.h>

iceLevel::iceLevel(){

}

iceLevel::~iceLevel(){

}

bool iceLevel::initialize(){

	return true;
}

void iceLevel::finalize(){

}

void iceLevel::update(){

}

bool iceLevel::createScene( Ogre::SceneManager* a_SceneMgr ){
	// Creating a DotSceneLoader
	DotSceneLoader pLoader;
	// Loading a Level. Level1 by default.
	pLoader.parseDotScene( "level1.scene","level1",a_SceneMgr );
	
	return true;
}