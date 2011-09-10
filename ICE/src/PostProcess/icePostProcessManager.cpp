#include <OGRE/Ogre.h>
#include "iceGame.h"

#include "PostProcess/icePostProcessManager.h"

template<> icePostProcessManager* Ogre::Singleton<icePostProcessManager>::ms_Singleton = 0;

icePostProcessManager::icePostProcessManager()
{
	mHardBlurCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "RadialHardBlur");
	mSoftBlurCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "RadialSoftBlur");

	mToonCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "ToonPostProcess");

	mDepthOfFieldCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "DepthOfField");
}

icePostProcessManager::~icePostProcessManager() 
{
	 Ogre::CompositorManager::getSingleton().removeCompositorChain(iceGame::getViewPort());
}

icePostProcessManager& icePostProcessManager::getSingleton()
{
    assert(ms_Singleton);
    return (*ms_Singleton);
}

icePostProcessManager* icePostProcessManager::getSingletonPtr()
{
    assert(ms_Singleton);
    return ms_Singleton;
}

void icePostProcessManager::enableHardBlur()
{
	mSoftBlurCompositor->setEnabled(false);
	mHardBlurCompositor->setEnabled(true);
}

void icePostProcessManager::enableSoftBlur()
{
	mHardBlurCompositor->setEnabled(false);
	mSoftBlurCompositor->setEnabled(true);
}

void icePostProcessManager::disableBlur()
{
	mHardBlurCompositor->setEnabled(false);
	mSoftBlurCompositor->setEnabled(false);
}

//Experimental
void icePostProcessManager::enableToon()
{
	//mToonCompositor->setEnabled(true);
}

//Experimental
void icePostProcessManager::disableToon()
{
	//mToonCompositor->setEnabled(false);
}

//Experimental
void icePostProcessManager::enableDepthOfField()
{
	//mDepthOfFieldCompositor->setEnabled(true);
}

//Experimental
void icePostProcessManager::disableDepthOfField()
{
	//mDepthOfFieldCompositor->setEnabled(false);
}
