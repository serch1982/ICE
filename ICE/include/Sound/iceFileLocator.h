#ifndef __ICE_FILE_LOCATOR_H__
#define __ICE_FILE_LOCATOR_H__

#include <Ogre.h>
#include <OgreResourceManager.h>

class iceFileLocator : public Ogre::ResourceGroupManager
{
public:
   iceFileLocator();
   ~iceFileLocator();
   Ogre::Archive* Find(Ogre::String &filename);
};

#endif