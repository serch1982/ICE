#ifndef ICEMUSIC_H_
#define ICEMUSIC_H_

#include <SDL_mixer.h>
#include <OGRE/Ogre.h>

 class iceMusic: public Ogre::Resource {
    public:
        /**
         *  Constructor
         */
        iceMusic(Ogre::ResourceManager* creator,
             const Ogre::String& name,
             Ogre::ResourceHandle handle,
             const Ogre::String& group,
             bool isManual = false,
             Ogre::ManualResourceLoader* loader = 0);

        /**
         *  Destructor
         */
        ~iceMusic();

        /**
         *  play
         */
        void play(int loop = -1);

        /**
         *  Pausa
         */
        void pause();

        /**
         *  stop
         */
        void stop();

        /**
         *  @param ms of the efect
         *  @param loop 
         */
        void fadeIn(int ms, int loop = -1);

        /**
         *  @param ms of the efect
         */
        void fadeOut(int ms);

        /**
         *  is a music is playing
         */
        static bool isPlaying();

		/**
		 *  set the volume and get the previous
		 */
		void setVolume( int vol );

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
        Mix_Music* _song;
        Ogre::String _path;
        size_t _size;
		int prevVolume;
};


/**
 *  ogre resource singleton
 */
class p_iceMusic: public Ogre::SharedPtr<iceMusic> {
    public:
        p_iceMusic(): Ogre::SharedPtr<iceMusic>() {}
        explicit p_iceMusic(iceMusic* m): Ogre::SharedPtr<iceMusic>(m) {}
        p_iceMusic(const p_iceMusic &m): Ogre::SharedPtr<iceMusic>(m) {}
        p_iceMusic(const Ogre::ResourcePtr &r);
        p_iceMusic& operator= (const Ogre::ResourcePtr& r);
};


#endif 
