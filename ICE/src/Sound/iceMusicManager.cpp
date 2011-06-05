#include "Sound\iceMusicManager.h"
#include "iceGame.h"

template<> iceMusicManager* Ogre::Singleton<iceMusicManager>::ms_Singleton = 0;

iceMusicManager::iceMusicManager() {
	_log = iceGame::getGameLog();
    mResourceType = "Music";
    mLoadOrder = 30.f;
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

iceMusicManager::~iceMusicManager() {
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	_log->logMessage("iceMusicManager::~iceMusicManager()");
}

p_iceMusic iceMusicManager::load(const Ogre::String& name, const Ogre::String& group) {
    p_iceMusic piceMusic = getByName(name);

    if (piceMusic.isNull()) 
        piceMusic = create(name, group);

    piceMusic->load();

    return piceMusic;
}

iceMusicManager& iceMusicManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

iceMusicManager* iceMusicManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

Ogre::Resource* iceMusicManager::createImpl(const Ogre::String& name,
                                          Ogre::ResourceHandle handle,
                                          const Ogre::String& group,
                                          bool isManual,
                                          Ogre::ManualResourceLoader* loader,
                                          const Ogre::NameValuePairList* createParams) {
    return new iceMusic(this, name, handle, group, isManual, loader);
}
