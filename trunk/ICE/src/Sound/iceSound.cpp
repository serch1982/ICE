#include <fstream>

#include "Sound\iceSound.h"
#include "iceGame.h"


iceSound::iceSound(Ogre::ResourceManager* creator,
                 const Ogre::String& name,
                 Ogre::ResourceHandle handle,
                 const Ogre::String& group,
                 bool isManual,
                 Ogre::ManualResourceLoader* loader):
                 Ogre::Resource(creator, name, handle, group, isManual, loader){
    createParamDictionary(name);
    _sound = 0;
    _path = "";
    _size = 0;
	_log = iceGame::getGameLog();
}

iceSound::~iceSound() {
    unload();
}

int iceSound::play(int loop) {
    int channel;

    if ((channel = Mix_PlayChannel(-1, _sound, loop)) == -1) {
       _log->logMessage("iceSound::play() cannot play");
    }

    return channel;
}


void iceSound::loadImpl() {
    Ogre::FileInfoListPtr info;
    info = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, mName);
    for (Ogre::FileInfoList::iterator i = info->begin(); i != info->end(); ++i) {
        _path = i->archive->getName() + "/" + i->filename;
    }

    if (_path == "") {
            _log->logMessage("iceSound::loadImpl() cannot find resource");
			return;
    }
	
    if ((_sound = Mix_LoadWAV(_path.c_str())) == NULL) {
            _log->logMessage("iceSound::loadImpl() cannot load");
			return;
    }

    // size
    std::ifstream stream;
    char byteBuffer;
    stream.open(_path.c_str(), std::ios_base::binary);
    
    while (stream >> byteBuffer)
        ++_size;

    stream.close();
}

void iceSound::unloadImpl() {
    if (_sound)
        Mix_FreeChunk(_sound);
}

size_t iceSound::calculateSize() const {
    return _size;
}


p_iceSound::p_iceSound(const Ogre::ResourcePtr& r) {
    if (r.isNull())
        return;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)

    pRep = static_cast<iceSound*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();

    if (pUseCount)
        ++(*pUseCount);
}

p_iceSound& p_iceSound::operator= (const Ogre::ResourcePtr& r) {
    if (pRep == static_cast<iceSound*>(r.getPointer()))
        return *this;

    release();

    if (r.isNull())
        return *this;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
    
    pRep = static_cast<iceSound*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();
    
    if (pUseCount)
        ++(*pUseCount);

    return *this;
}

