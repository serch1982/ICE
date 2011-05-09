#ifndef ICESOUND_H_
#define ICESOUND_H_

#include <SDL_mixer.h>
#include <Ogre.h>

class iceSound: public Ogre::Resource {
    public:
        /**
         *  Constructor
         */
        iceSound(Ogre::ResourceManager* creator,
               const Ogre::String& name,
               Ogre::ResourceHandle handle,
               const Ogre::String& group,
               bool isManual = false,
               Ogre::ManualResourceLoader* loader = 0);

        /**
         *  Destructor
         */
        ~iceSound();

        /**
         * play
         */
        int play(int loop = 0);
        
    protected:
        /**
         *  load 
         */
        void loadImpl();

        /**
         *  unload
         */
        void unloadImpl();

        /**
         *  memory size
         */
        size_t calculateSize() const;
    private:
		Ogre::Log* _log;
        Mix_Chunk* _sound;
        Ogre::String _path;
        size_t _size;
};


/**
 *  ogre resource singleton
 */
class p_iceSound: public Ogre::SharedPtr<iceSound> {
    public:
        p_iceSound(): Ogre::SharedPtr<iceSound>() {}
        explicit p_iceSound(iceSound* s): Ogre::SharedPtr<iceSound>(s) {}
        p_iceSound(const p_iceSound& s): Ogre::SharedPtr<iceSound>(s) {}
        p_iceSound(const Ogre::ResourcePtr& r);
        p_iceSound& operator= (const Ogre::ResourcePtr& r);
};

#endif 
