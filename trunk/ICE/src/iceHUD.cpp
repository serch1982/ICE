	#include "iceHUD.h"
#include "iceState.h"

iceHUD* iceHUD::pinstance = 0;

iceHUD* iceHUD::getInstance(){
	if (pinstance == 0)
          pinstance = new iceHUD;
        return pinstance;
}


iceHUD::iceHUD(void){
	//mGameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
	isLoad = false;
}

iceHUD::~iceHUD(void){
   
}


bool iceHUD::setupHUD(char* path, char* name, Ogre::Viewport* mViewport)
{
	mGameLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
	try{
		if(!isLoad){
			hikariMgrHUD = iceMenu::getInstance()->getHikariManager(); 
			hikariHUD = hikariMgrHUD->createFlashOverlay("HUD", mViewport, mViewport->getActualWidth(), mViewport->getActualHeight(), Hikari::Position(Hikari::Center));
			hikariHUD->load(name); 
			hikariHUD->setTransparent(true, true);
			isLoad = true;
		}
		return true;
	}catch(char* ex) {
		mGameLog->logMessage(ex);
		return false;
	}
	
}


void iceHUD::show(){
	hikariHUD->show();
}

void iceHUD::hide(){
	hikariHUD->hide();
}

void iceHUD::mouseMoved(const OIS::MouseEvent &arg){
	
}

void iceHUD::mouseDown(OIS::MouseButtonID id){

}

void iceHUD::mouseUp(OIS::MouseButtonID id){
	
}

void iceHUD::update(){
	if(hikariHUD->getVisibility() == false){
		hikariHUD->show();
	}
	hikariMgrHUD->update();
}

// set the life, it must be between 0 - 100
void iceHUD::setLife(int life){
	hikariHUD->callFunction("setLife",Hikari::Args(life));
}

//set the name of the current weapon 
void iceHUD::setWeapon(char* name){
	hikariHUD->callFunction("setWeapon", Hikari::Args(name));
}