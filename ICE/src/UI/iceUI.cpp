#include "UI/iceUI.h"
#include "iceGame.h"

iceUI::iceUI()
{
	iceGame::getGameLog()->logMessage("iceUI::iceUI()");
	mHikariMgr = new Hikari::HikariManager("media");
	mMenu = new iceMenu(mHikariMgr);
	mHUD = new iceHUD();
}

iceUI::~iceUI()
{
	iceGame::getGameLog()->logMessage("iceUI::~iceUI()");
	delete mMenu;
	delete mHUD;
	mHikariMgr->destroyAllControls();
	delete mHikariMgr;
}

void iceUI::init(iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceUI::init()");

	mStateManager = pStateManager;

	mMenu->init(mStateManager);
	mHUD->init(mStateManager);
}

void iceUI::update(Ogre::Real evt)
{
	if(mStateManager->getCurrentStateID() == MAINMENU) mHikariMgr->update();
	if(mStateManager->getCurrentStateID() == PAUSE) mHikariMgr->update();
	if(mStateManager->getCurrentStateID() == PLAY) mHUD->update(evt);
}

iceMenu* iceUI::getMenu()
{
	return mMenu;
}

iceHUD* iceUI::getHUD()
{
	return mHUD;
}