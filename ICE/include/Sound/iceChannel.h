#ifndef __ICE_CHANNEL_H__
#define __ICE_CHANNEL_H__

#include <Ogre.h>

class iceChannel
{
public:
   void              Clear(void);
   Ogre::SceneNode * sceneNode;
   //Ogre::Vector3	 curPosition;
   Ogre::Vector3     prevPosition;
};

#endif