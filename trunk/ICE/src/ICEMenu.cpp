#include "ICEMenu.h"

ICEMenu::ICEMenu(void){
	mMENUSTATE = MENU;
}

ICEMenu::~ICEMenu(void){
   if (hikariMgr) delete hikariMgr;
}

void ICEMenu::setupHikari(char* path, char* name, Ogre::Viewport* mViewport, int wight, int height)
{
	hikariMgr = new Hikari::HikariManager(path); //"..\\..\\media"
	hikariMenu = hikariMgr->createFlashOverlay("menu", mViewport, wight, height, Hikari::Position(Hikari::Center));
	hikariMenu->load(name); //"menu.swf"
	hikariMenu->setTransparent(false, true);
	hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &ICEMenu::menuExitClick));
	hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &ICEMenu::menuPlayClick));
	hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &ICEMenu::menuContinueClick));
	ShowCursor(true);
}

Hikari::FlashValue ICEMenu::menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	mMENUSTATE = EXIT;
	return FLASH_VOID;
}

Hikari::FlashValue ICEMenu::menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	ShowCursor(false);
	mMENUSTATE = PLAY;
	hikariMenu->hide();
	return FLASH_VOID;
}

Hikari::FlashValue ICEMenu::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	ShowCursor(false);
	mMENUSTATE = CONTINUE;
	hikariMenu->hide();
	return FLASH_VOID;
}

ICEMenu::MENUSTATE ICEMenu::getState(){
	return mMENUSTATE;
}

void ICEMenu::showMenu(){
	ShowCursor(true);
	mMENUSTATE = MENU;
	hikariMenu->show();
}

void ICEMenu::mouseMoved(const OIS::MouseEvent &arg){
	hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || hikariMgr->injectMouseWheel(arg.state.Z.rel);
}

void ICEMenu::mouseDown(OIS::MouseButtonID id){
	hikariMgr->injectMouseDown(id);
}

void ICEMenu::mouseUp(OIS::MouseButtonID id){
	hikariMgr->injectMouseUp(id);
}

void ICEMenu::update(){
	hikariMgr->update();
}