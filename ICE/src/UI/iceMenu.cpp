#include <UI/iceMenu.h>
#include "iceGame.h"

iceMenu::iceMenu(Hikari::HikariManager* pHikariMgr)
{
	iceGame::getGameLog()->logMessage("iceMenu::iceMenu()");
	mHikariMgr = pHikariMgr;
}

iceMenu::~iceMenu()
{
	iceGame::getGameLog()->logMessage("iceMenu::~iceMenu()");
}

void iceMenu::init(iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceMenu::init()");

	mStateManager = pStateManager;

	try{
		mFlash = mHikariMgr->createFlashOverlay("MENU", iceGame::getCamera()->getViewport(),  iceGame::getCamera()->getViewport()->getActualWidth(),  iceGame::getCamera()->getViewport()->getActualHeight(), Hikari::Position(Hikari::Center));
		mFlash->load("menu.swf");
		mFlash->setTransparent(false, true);
		mFlash->bind("menuExitClick", Hikari::FlashDelegate(this, &iceMenu::exitClick));
		mFlash->bind("menuPlayClick", Hikari::FlashDelegate(this, &iceMenu::playClick));
		mFlash->bind("menuContinueClick", Hikari::FlashDelegate(this, &iceMenu::continueClick));
		mFlash->hide();
	}catch(char* ex) {
		iceGame::getGameLog()->logMessage(ex);
	}
}

Hikari::FlashValue iceMenu::exitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	mStateManager->menuExitClick();
	return FLASH_VOID;
}

Hikari::FlashValue iceMenu::playClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	mStateManager->menuPlayClick();
	return FLASH_VOID;
}

Hikari::FlashValue iceMenu::continueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	mStateManager->menuContinueClick();
	return FLASH_VOID;
}

bool iceMenu::keyPressed(const OIS::KeyEvent &arg) {
    return true;
}

bool iceMenu::keyReleased(const OIS::KeyEvent &arg) {
    return true;
}

bool iceMenu::mouseMoved(const OIS::MouseEvent &arg) {
	iceSdkTray::getInstance()->moveMouse(arg);
	mHikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || mHikariMgr->injectMouseWheel(arg.state.Z.rel);
    return true;
}

bool iceMenu::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	mHikariMgr->injectMouseDown(id);
    return true;
}

bool iceMenu::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	mHikariMgr->injectMouseUp(id);
    return true;
}

void iceMenu::showMainMenu()
{
	mFlash->show();
	mFlash->callFunction("inGame",Hikari::Args(false));
}

void iceMenu::showPauseMenu()
{
	mFlash->show();
	mFlash->callFunction("inGame",Hikari::Args(true));
}

void iceMenu::hide()
{
	mFlash->hide();
}