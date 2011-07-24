
#ifndef _ICE_HIKARI_OBJECT_H_
#define _ICE_HIKARI_OBJECT_H_

#include <OGRE/Ogre.h>
#include <Hikari.h>
#include "../States/iceStateManager.h"

class iceHikariObject
{
    public:
        iceHikariObject(Hikari::HikariManager* pHikariMgr, iceStateManager* pStateManager);
		~iceHikariObject();

		virtual void init(Ogre::String pFileName);

	protected:
		iceStateManager* mStateManager;
		Hikari::HikariManager* mHikariMgr;
		Hikari::FlashControl* mFlash;
		Ogre::String mObjectName;
};

#endif  
