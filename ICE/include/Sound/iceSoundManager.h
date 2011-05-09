
#ifndef ICESOUNDMANAGER_H_
#define ICESOUNDMANAGER_H_

#include <OGRE/Ogre.h>

#include "iceSound.h"


class iceSoundManager: public Ogre::ResourceManager,
                      public Ogre::Singleton<iceSoundManager> {
    public:
        /**
         *  Constructor
         */
        iceSoundManager();

        /**
         *  Destructor
         */
        virtual ~iceSoundManager();

        /**
         * instance pointer
         */
        virtual p_iceSound load(const Ogre::String& name,
                                const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        /**
         *  referencence instance
         */
        static iceSoundManager& getSingleton();

        /**
         *  pointer
         */
        static iceSoundManager* getSingletonPtr();

        /**
         *  @return number of channels availables
         */
        static int getAvailableChannels();
    protected:
        Ogre::Resource* createImpl(const Ogre::String& name,
                                   Ogre::ResourceHandle handle,
                                   const Ogre::String& group,
                                   bool isManual,
                                   Ogre::ManualResourceLoader* loader,
                                   const Ogre::NameValuePairList* createParams);
    
    private:
        static int numChannels;
		Ogre::Log* _log;
};

#endif  
