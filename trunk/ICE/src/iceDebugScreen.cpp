#include "iceDebugScreen.h"

iceDebugScreen* iceDebugScreen::pinstance = 0;
iceDebugScreen* iceDebugScreen::instance(){
	if (pinstance == 0)
          pinstance = new iceDebugScreen;
        return pinstance;
}


iceDebugScreen::iceDebugScreen(void){

}

iceDebugScreen::~iceDebugScreen(void){
   if (mTrayMgr) delete mTrayMgr;
}


void iceDebugScreen::setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener){
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, winListener);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, sdkListener);
    mTrayMgr->hideCursor();

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
	items.push_back("test LUA");				// 8
	items.push_back("Weapon");					// 9
	
	/*
		here you can add your custom chivato like items.push_back("mycustomchivato"); 
	*/

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", items.size() * 25, items);

#ifdef OGRE_DEBUG_MODE  
    mDetailsPanel->show();
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
#else
    mDetailsPanel->hide();
    mTrayMgr->hideAll();
#endif

    iChivatos = items.size();
}


void iceDebugScreen::updateChivato( int paramNumber, Ogre::String paramValue){
    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible()) 
        {
            if ((paramNumber >= 0) && (paramNumber < iChivatos)) {
                mDetailsPanel->setParamValue(paramNumber, paramValue);
            }
        }
    }
}

void iceDebugScreen::updateFrameEvent(const Ogre::FrameEvent& evt){
	mTrayMgr->frameRenderingQueued(evt);
}


void iceDebugScreen::showFrameStats(){
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void iceDebugScreen::hideFrameStats(){
    mTrayMgr->hideFrameStats();
}

void iceDebugScreen::hideChivatos(){
    mDetailsPanel->hide();
}

void iceDebugScreen::showChivatos(){
    mDetailsPanel->show();
}

bool iceDebugScreen::isDialogVisible(){
    return mTrayMgr->isDialogVisible();
}