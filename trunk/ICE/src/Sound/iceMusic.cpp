#include <fstream>
#include <iostream>

#include "Sound\iceMusic.h"

iceMusic::iceMusic(Ogre::ResourceManager* creator,
               const Ogre::String& name,
               Ogre::ResourceHandle handle,
               const Ogre::String& group,
               bool isManual,
               Ogre::ManualResourceLoader* loader):
               Ogre::Resource(creator, name, handle, group, isManual, loader) {
	 _log = Ogre::LogManager::getSingleton().getLog("iceLog.log");
    createParamDictionary(name);
    _song = 0;
    _size = 0;
}

iceMusic::~iceMusic() {
    unload();
}

void iceMusic::play(int loop) {
    if(Mix_PausedMusic())
            Mix_ResumeMusic();
    else{
        if(Mix_PlayMusic(_song, loop) == -1){
			 _log->logMessage("iceMusic::play() cannot play");
        }
    }
}

void iceMusic::pause() {
    Mix_PauseMusic();
}

void iceMusic::stop() {
    Mix_HaltMusic();
}

void iceMusic::setVolume( int vol ){
	prevVolume = Mix_VolumeMusic( vol );
}

/*void iceMusic::restoreVolume(){
	prevVolume = Mix_VolumeMusic( prevVolume );
}*/

void iceMusic::fadeIn(int ms, int loop) {
    if (Mix_FadeInMusic(_song, ms, loop) == -1) {
            _log->logMessage("iceMusic::fadeIn() cannot play");
    }
}

void iceMusic::fadeOut(int ms) {
    if (Mix_FadeOutMusic(ms) == -1) {
            _log->logMessage("iceMusic::fadeOut() cannot play");
    }
}

bool iceMusic::isPlaying() {
    return Mix_PlayingMusic()? true : false;
}

void iceMusic::loadImpl() {
    _path = "";

    Ogre::FileInfoListPtr info;
    info = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, mName);
    for (Ogre::FileInfoList::iterator i = info->begin(); i != info->end(); ++i) {
        _path = i->archive->getName() + "/" + i->filename;
    }

    if (_path == "") {
            _log->logMessage("iceMusic::loadImpl() cannot find resource");
			return;
    }

    if ((_song = Mix_LoadMUS(_path.c_str())) == NULL) {
            _log->logMessage("iceMusic::loadImpl() cannot load");
			return;
    }

    // file size
    std::ifstream stream;
    char byteBuffer;
    stream.open(_path.c_str(), std::ios_base::binary);
    
    while (stream >> byteBuffer)
        ++_size;
    stream.close();
}

void iceMusic::unloadImpl() {
    if (_song)
        Mix_FreeMusic(_song);
}

size_t iceMusic::calculateSize() const {
    return _size;
}

//------------------------------------ 

p_iceMusic::p_iceMusic(const Ogre::ResourcePtr &r): Ogre::SharedPtr<iceMusic>() {
    if (r.isNull())
        return;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)

    pRep = static_cast<iceMusic*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();

    if (pUseCount)
        ++(*pUseCount);

}

p_iceMusic& p_iceMusic::operator= (const Ogre::ResourcePtr& r) {
    if (pRep == static_cast<iceMusic*>(r.getPointer()))
        return *this;

    release();

    if (r.isNull())
        return *this;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
    
    pRep = static_cast<iceMusic*>(r.getPointer());
    pRep = static_cast<iceMusic*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();
    
    if (pUseCount)
        ++(*pUseCount);

    return *this;
}
