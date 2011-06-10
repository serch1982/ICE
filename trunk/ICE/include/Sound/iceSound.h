#ifndef __ICE_SOUND_H__
#define __ICE_SOUND_H__

#include <Ogre.h>
#include <fmod.hpp>

typedef enum
   {
   SOUND_TYPE_INVALID = 0,
   SOUND_TYPE_3D_SOUND,
   SOUND_TYPE_3D_SOUND_LOOPED,
   SOUND_TYPE_2D_SOUND,
   SOUND_TYPE_2D_SOUND_LOOPED,
   } SOUND_TYPE;


class iceSound
{
public:
   void           Clear(void);
   Ogre::String   fileName;
   SOUND_TYPE     soundType;
   FMOD::Sound *  fmodSound;
};

#endif