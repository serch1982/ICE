#include "Utils\iceSdkTray.h"

iceSdkTray* iceSdkTray::pinstance = 0;
iceSdkTray* iceSdkTray::getInstance(){
	if (pinstance == 0)
          pinstance = new iceSdkTray;
        return pinstance;
}


iceSdkTray::iceSdkTray(void){

}

iceSdkTray::~iceSdkTray(void){
   if (mTrayMgr) delete mTrayMgr;
}


void iceSdkTray::setupScreenInfo(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener){
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, winListener);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, new OgreBites::SdkTrayListener);
    mTrayMgr->showCursor();


	// my chivatos
    Ogre::StringVector items;
    items.push_back("cam.pX");					// 0
    items.push_back("cam.pY");					// 1
    items.push_back("cam.pZ");					// 2
    items.push_back("cam.oW");					// 3
    items.push_back("cam.oX");					// 4
    items.push_back("cam.oY");					// 5
    items.push_back("cam.oZ");					// 6
    items.push_back("Poly Mode");				// 7
	items.push_back("sprint x-y-t");				// 8
	items.push_back("Weapon");					// 9
	items.push_back("PlayerX");					// 10
	items.push_back("PlayerY");					// 11
	items.push_back("FrameX");					// 12
	items.push_back("FrameY");					// 13
	items.push_back("Level");					// 14
	items.push_back("cam dir");					// 15
	items.push_back("mouse");					// 16
	items.push_back("ship");					// 17
	items.push_back("coli");					// 18
	items.push_back("speed");					// 19
	items.push_back("currTime");				// 20
	/*
		here you can add your custom chivato like items.push_back("mycustomchivato"); 
	*/

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 300, items);

#ifdef _DEBUG
    mDetailsPanel->show();
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
#else
    mDetailsPanel->hide();
    mTrayMgr->hideAll();
#endif

    iItems = items.size();
}


void iceSdkTray::updateScreenInfo( int paramNumber, Ogre::String paramValue){
    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible()) 
        {
            if ((paramNumber >= 0) && (paramNumber < iItems)) {
                mDetailsPanel->setParamValue(paramNumber, paramValue);
            }
        }
    }
}

void iceSdkTray::updateFrameEvent(const Ogre::FrameEvent& evt){
	mTrayMgr->frameRenderingQueued(evt);
}


void iceSdkTray::showFrameStats(){
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void iceSdkTray::hideFrameStats(){
    mTrayMgr->hideFrameStats();
}

void iceSdkTray::hideInfo(){
    mDetailsPanel->hide();
}

void iceSdkTray::showInfo(){
    mDetailsPanel->show();
}

bool iceSdkTray::isDialogVisible(){
    return mTrayMgr->isDialogVisible();
}

void iceSdkTray::showCursor(){
    mTrayMgr->showCursor();
}

void iceSdkTray::hideCursor(){
    mTrayMgr->hideCursor();
}

void iceSdkTray::moveMouse(const OIS::MouseEvent &evt){
	mTrayMgr->injectMouseMove(evt);
}