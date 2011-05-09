#ifndef ICEMUSICMANAGER_H_
#define ICEMUSICMANAGER_H_

#include <OGRE/Ogre.h>

#include "Sound\iceMusic.h"

class iceMusicManager: public Ogre::ResourceManager, public Ogre::Singleton<iceMusicManager> {
    public:
        /**
         *  constructor
         */
        iceMusicManager();

        /**
         *  Destructor
         */
        virtual ~iceMusicManager();

        /**
         *  @param name file name
         *  @group grupo del recurso. 
         *  @return shared pointer iceMusic
         */
        virtual p_iceMusic load(const Ogre::String& name, const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        /**
         * instance
         */
        static iceMusicManager& getSingleton();

        /**
         *  instance
         */
        static iceMusicManager* getSingletonPtr();

    protected:
        /**
         *  @param name file name
         *  @param handle
         *  @param group name
         *  @param isManual 
         *  @param loader 
         *  @param createParams config
         */
        Ogre::Resource* createImpl(const Ogre::String& name,
                                   Ogre::ResourceHandle handle,
                                   const Ogre::String& group,
                                   bool isManual,
                                   Ogre::ManualResourceLoader* loader,
                                   const Ogre::NameValuePairList* createParams);
		private:
			Ogre::Log* _log;
};

#endif  
