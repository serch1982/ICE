#include "iceMenu.h"
#include "iceState.h"

iceMenu* iceMenu::pinstance = 0;

iceMenu* iceMenu::getInstance(){
	if (pinstance == 0)
          pinstance = new iceMenu;
        return pinstance;
}


iceMenu::iceMenu(void){
	//mGameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
}

iceMenu::~iceMenu(void){
   
}


bool iceMenu::setupHikari(char* path, char* name, Ogre::Viewport* mViewport)
{
	iceState::getInstance()->setState( iceState::MENU );
	mGameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
	try{
		hikariMgr = new Hikari::HikariManager(path);
		hikariMenu = hikariMgr->createFlashOverlay("menu", mViewport, mViewport->getActualWidth(), mViewport->getActualHeight(), Hikari::Position(Hikari::Center));
		hikariMenu->load(name); //"menu.swf"
		hikariMenu->setTransparent(false, true);
		hikariMenu->bind("menuExitClick", Hikari::FlashDelegate(this, &iceMenu::menuExitClick));
		hikariMenu->bind("menuPlayClick", Hikari::FlashDelegate(this, &iceMenu::menuPlayClick));
		hikariMenu->bind("menuContinueClick", Hikari::FlashDelegate(this, &iceMenu::menuContinueClick));
		return true;
	}catch(char* ex) {
		mGameLog->logMessage(ex);
		return false;
	}
	
}

Hikari::FlashValue iceMenu::menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	iceState::getInstance()->setState( iceState::EXIT );
	return FLASH_VOID;
}

Hikari::FlashValue iceMenu::menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	hikariMenu->callFunction("inGame",Hikari::Args(true));
	iceState::getInstance()->setState( iceState::LOAD_LEVEL );
	hikariMenu->hide();
	return FLASH_VOID;
}

Hikari::FlashValue iceMenu::menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	hikariMenu->callFunction("inGame",Hikari::Args(true));
	iceState::getInstance()->setState( iceState::CONTINUE );
	hikariMenu->hide();
	return FLASH_VOID;
}

void iceMenu::show(){
	hikariMenu->show();
}

void iceMenu::hide(){
	hikariMenu->hide();
}

void iceMenu::mouseMoved(const OIS::MouseEvent &arg){
	hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || hikariMgr->injectMouseWheel(arg.state.Z.rel);
}

void iceMenu::mouseDown(OIS::MouseButtonID id){
	hikariMgr->injectMouseDown(id);
}

void iceMenu::mouseUp(OIS::MouseButtonID id){
	hikariMgr->injectMouseUp(id);
}

void iceMenu::update(){
	hikariMgr->update();
}


Hikari::HikariManager* iceMenu::getHikariManager(){
	return hikariMgr;
}