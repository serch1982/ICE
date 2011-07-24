#include <UI/iceHikariObject.h>
#include "iceGame.h"

iceHikariObject::iceHikariObject(Hikari::HikariManager* pHikariMgr, iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceHikariObject::iceHikariObject()");
	mHikariMgr = pHikariMgr;
	mStateManager = pStateManager;
}

iceHikariObject::~iceHikariObject()
{
	iceGame::getGameLog()->logMessage("iceHikariObject::~iceHikariObject()");
}

void iceHikariObject::init(Ogre::String pFileName)
{
	iceGame::getGameLog()->logMessage("iceHikariObject::init()");

	try{
		mFlash = mHikariMgr->createFlashOverlay(mObjectName, iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
		mFlash->load(pFileName);
	}catch(char* ex) {
		iceGame::getGameLog()->logMessage(ex);
	}
}