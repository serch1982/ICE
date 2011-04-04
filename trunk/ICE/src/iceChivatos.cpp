#include "iceChivatos.h"

iceChivatos* iceChivatos::pinstance = 0;
iceChivatos* iceChivatos::instance(){
	if (pinstance == 0)
          pinstance = new iceChivatos;
        return pinstance;
}


iceChivatos::iceChivatos(void){

}

iceChivatos::~iceChivatos(void){
   if (mTrayMgr) delete mTrayMgr;
}


void iceChivatos::setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener){
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
	items.push_back("Machinegun Ammo");			// 9
	items.push_back("Shotgun Ammo");			// 10
	items.push_back("Misile launcher Ammo");	// 11
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


void iceChivatos::updateChivato( int paramNumber, Ogre::String paramValue){
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

void iceChivatos::updateFrameEvent(const Ogre::FrameEvent& evt){
	mTrayMgr->frameRenderingQueued(evt);
}


void iceChivatos::showFrameStats(){
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void iceChivatos::hideFrameStats(){
    mTrayMgr->hideFrameStats();
}

void iceChivatos::hideChivatos(){
    mDetailsPanel->hide();
}

void iceChivatos::showChivatos(){
    mDetailsPanel->show();
}

bool iceChivatos::isDialogVisible(){
    return mTrayMgr->isDialogVisible();
}