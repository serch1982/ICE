#include "ICEMenu.h"

ICEMenu* ICEMenu::pinstance = 0;

ICEMenu* ICEMenu::instance(){
	if (pinstance == 0)
          pinstance = new ICEMenu;
        return pinstance;
}


ICEMenu::ICEMenu(void){

}

ICEMenu::~ICEMenu(void){
   
}

bool ICEMenu::setupHikari(char* path, char* name, Ogre::Viewport* mViewport, int wight, int height)
{
	mMENUSTATE = MENU;
	try{
		hikariMgr = new Hikari::HikariManager(path); //".\\media"
		hikariMenu = hikariMgr->createFlashOverlay("menu", mViewport, wight, height, Hikari::Position(Hikari::Center));
		hikariMenu->load(name); //"menu.swf"
		hikariMenu->setTransparent(false, true);
		hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &ICEMenu::menuExitClick));
		hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &ICEMenu::menuPlayClick));
		hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &ICEMenu::menuContinueClick));
		ShowCursor(true);
		return true;
	}catch(char* ex) {
		return false;
	}
	
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
	hikariMenu->callFunction("inGame",Hikari::Args(true));
	hikariMenu->hide();
	return FLASH_VOID;
}

Hikari::FlashValue ICEMenu::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	ShowCursor(false);
	hikariMenu->callFunction("inGame",Hikari::Args(true));
	mMENUSTATE = CONTINUE;
	hikariMenu->hide();
	return FLASH_VOID;
}

ICEMenu::MENUSTATE ICEMenu::getState(){
	return mMENUSTATE;
}

void ICEMenu::setState(ICEMenu::MENUSTATE state){
	mMENUSTATE =state;
	switch(state){
		case MENU:
			ShowCursor(true);
			hikariMenu->show();
			break;
		case PLAY:
		case CONTINUE:
			ShowCursor(false);
			hikariMenu->hide();
			break;	
	}
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