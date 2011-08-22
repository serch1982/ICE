
#ifndef ICEPOSTPROCESSMANAGER_H_
#define ICEPOSTPROCESSMANAGER_H_

#include <OGRE/Ogre.h>

class icePostProcessManager: public Ogre::Singleton<icePostProcessManager> {
    public:
        /**
         *  Constructor
         */
        icePostProcessManager();

        /**
         *  Destructor
         */
        ~icePostProcessManager();

        /**
         *  @return instance manager
         */
        static icePostProcessManager& getSingleton();

        /**
         *  @return instance pointer manager
         */
        static icePostProcessManager* getSingletonPtr();

		void enableHardBlur();;
		void enableSoftBlur();
		void disableBlur();

		void enableToon();
		void disableToon();

    private:

		Ogre::CompositorInstance* mHardBlurCompositor;
		Ogre::CompositorInstance* mSoftBlurCompositor;

		Ogre::CompositorInstance* mToonCompositor;
        
};

#endif  
