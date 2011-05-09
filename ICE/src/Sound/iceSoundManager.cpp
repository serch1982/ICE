#include <SDL_mixer.h>

#include "Sound\iceSoundManager.h"

template<> iceSoundManager* Ogre::Singleton<iceSoundManager>::ms_Singleton = 0;

int iceSoundManager::numChannels = 32;
        
iceSoundManager::iceSoundManager() {
	_log = Ogre::LogManager::getSingleton().getLog("iceLog.log");
    mResourceType = "Sound";
    mLoadOrder = 30.f;
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    Mix_AllocateChannels(numChannels);
}

iceSoundManager::~iceSoundManager() {
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	_log->logMessage("iceSoundManager::~iceSoundManager()");
}

p_iceSound iceSoundManager::load(const Ogre::String& name, const Ogre::String& group) {
    p_iceSound sound = getByName(name);

    if (sound.isNull())
        sound = create(name, group);
    
    sound->load();

    return sound;
}

iceSoundManager& iceSoundManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

iceSoundManager* iceSoundManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

int iceSoundManager::getAvailableChannels() {
    return numChannels;
}

Ogre::Resource* iceSoundManager::createImpl(const Ogre::String& name,
                                           Ogre::ResourceHandle handle,
                                           const Ogre::String& group,
                                           bool isManual,
                                           Ogre::ManualResourceLoader* loader,
                                           const Ogre::NameValuePairList* createParams) {
    return new iceSound(this, name, handle, group, isManual, loader);
}


