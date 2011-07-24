#include <UI/iceHUD.h>
#include "iceGame.h"

iceHUD::iceHUD(Hikari::HikariManager* pHikariMgr)
{
	iceGame::getGameLog()->logMessage("iceMenu::iceMenu()");
	mHikariMgr = pHikariMgr;
}

iceHUD::~iceHUD()
{
	iceGame::getGameLog()->logMessage("iceMenu::~iceMenu()");
}

void iceHUD::init(iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceMenu::init()");

	mStateManager = pStateManager;

	try{
		mFlash = mHikariMgr->createFlashOverlay("HUD", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
		mFlash->load("HUD.swf");
		mFlash->setTransparent(true, true);
		mFlash->hide();
	}catch(char* ex) {
		iceGame::getGameLog()->logMessage(ex);
	}
}

void iceHUD::show()
{
	mFlash->show();
	mFlash->gotoFrame(0);
}

void iceHUD::hide()
{
	mFlash->hide();
}