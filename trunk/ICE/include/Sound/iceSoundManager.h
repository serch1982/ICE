
#ifndef __ICESOUNDMANAGER_H__
#define __ICESOUNDMANAGER_H__

#include <Ogre.h>
#include "iceSound.h"
#include "iceChannel.h"
#include "iceFileLocator.h"
#include <fmod.hpp>

#define MAX_SOUND_CHANNELS       200

#define INVALID_SOUND_INDEX      -2
#define INVALID_SOUND_CHANNEL    -1

class iceSoundManager : public Ogre::Singleton<iceSoundManager>
   {
public:
                        iceSoundManager();
   virtual             ~iceSoundManager();
   bool                 Initialize(void);
   void                 StopAllSounds(void);
   void                 FrameStarted(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed);
   void					loadResources();

   int                  CreateSound(Ogre::String &fileName);         // single-shot 3D sound.  returns soundIndex
   int                  CreateStream(Ogre::String &fileName);        // single-shot 2D stream.  returns soundIndex
   int                  CreateLoopedSound(Ogre::String &fileName);   // looping 3D sound.  returns soundIndex
   int                  CreateLoopedStream(Ogre::String &fileName);  // looping 2D stream.  returns soundIndex

   int                  CreateSound(Ogre::String &fileName, SOUND_TYPE soundType);

   void                 PlaySound(int soundIndex, Ogre::SceneNode *soundNode, int *channelIndex);
   void                 StopSound(int *channelIndex);
   int                  FindSound(Ogre::String &fileName, SOUND_TYPE soundType);    // returns soundIndex;

   void                 Set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance);

   float                GetSoundLength(int soundIndex);        // returns seconds

   static iceSoundManager& getSingleton(void);
   static iceSoundManager* getSingletonPtr(void);

private:
   typedef std::vector<iceSound *>   SoundVector;
   typedef SoundVector::iterator  SoundVectorItr;

   int                  nextSoundInstanceIndex;
   FMOD::System         *mFMODSystem;
   Ogre::Vector3        mPrevListenerPosition;
   SoundVector			*mSoundVector;
   iceChannel		    mChannelArray[MAX_SOUND_CHANNELS];

   };

#endif // __SOUNDMANAGER_H__