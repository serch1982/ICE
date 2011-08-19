#include <OGRE/Ogre.h>
#include "iceGame.h"

#include "PostProcess/icePostProcessManager.h"

template<> icePostProcessManager* Ogre::Singleton<icePostProcessManager>::ms_Singleton = 0;

icePostProcessManager::icePostProcessManager()
{
	mHardBlurCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "RadialHardBlur");
	mSoftBlurCompositor = Ogre::CompositorManager::getSingleton().addCompositor(iceGame::getViewPort(), "RadialSoftBlur");
}

icePostProcessManager::~icePostProcessManager()
{
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
