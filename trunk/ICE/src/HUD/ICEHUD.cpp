#include "HUD\ICEHUD.h"

ICEHUD* ICEHUD::pinstance = 0;
ICEHUD* ICEHUD::instance(){
	if (pinstance == 0)
          pinstance = new ICEHUD;
        return pinstance;
}


ICEHUD::ICEHUD(void){

}

ICEHUD::~ICEHUD(void){
   if (mTrayMgr) delete mTrayMgr;
}


bool ICEHUD::setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener){
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, winListener);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, sdkListener);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

	// create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("test");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(2, "Bilinear");
    mDetailsPanel->setParamValue(3, "Solid");
    mDetailsPanel->show();

	return true;
}

void ICEHUD::updateChivato(const Ogre::FrameEvent& evt, Ogre::String paramName, Ogre::String paramValue){
	mTrayMgr->frameRenderingQueued(evt);

		if (!mTrayMgr->isDialogVisible())
		{
			if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
			{
				if(vChivatos.size > 0){
					mDetailsPanel->setParamValue(0, "variable");
				}
			}
		}
}


void ICEHUD::showFrameStats(){
	mTrayMgr->toggleAdvancedFrameStats();
}