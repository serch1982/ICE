
#ifndef __ICESOUNDMANAGER_H__
#define __ICESOUNDMANAGER_H__

#include <Ogre.h>
#include "iceSound.h"
#include "iceChannel.h"
#include "iceFileLocator.h"
#include <fmod.hpp>

// Maximum channels
#define MAX_SOUND_CHANNELS       200

// values for check errors
#define INVALID_SOUND_INDEX      -2
#define INVALID_SOUND_CHANNEL    -1

class iceSoundManager : public Ogre::Singleton<iceSoundManager>
   {
public:
                        iceSoundManager();
   virtual             ~iceSoundManager();
   bool                 Initialize(void);
   void                 StopAllSounds(void);
   // The new update function
   void					update( Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Real timeElapsed );
   //The update function
   //void                 FrameStarted(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed);
   // Auxiliar update function. Just test.
   //void					update( Ogre::Real timeElapsed );

   // Load Menu sounds
   void					loadResourcesMenu();

   // Create sounds functions
   int                  CreateSound(Ogre::String &fileName);         // single-shot 3D sound.  returns soundIndex
   int                  CreateStream(Ogre::String &fileName);        // single-shot 2D stream.  returns soundIndex
   int                  CreateLoopedSound(Ogre::String &fileName);   // looping 3D sound.  returns soundIndex
   int                  CreateLoopedStream(Ogre::String &fileName);  // looping 2D stream.  returns soundIndex
   // The function that really load and fill sounds
   int                  CreateSound(Ogre::String &fileName, SOUND_TYPE soundType);

   // Sound interaction
   void iceSoundManager::PlaySound(int soundIndex, Ogre::Vector3 soundPosition, int *channelIndex);
   //void                 PlaySound(int soundIndex, Ogre::SceneNode *soundNode, int *channelIndex);
   void                 StopSound(int *channelIndex);

   // Basic finder function. Returns the index of the vector
   int                  FindSound(Ogre::String &fileName, SOUND_TYPE soundType);    // returns soundIndex;

   // 3D parameters
   void                 Set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance);

   // ¿? today unused
   float                GetSoundLength(int soundIndex);        // returns seconds

   // this class is Singleton
   static iceSoundManager& getSingleton(void);
   static iceSoundManager* getSingletonPtr(void);

private:
   //Rename some types
   typedef std::vector<iceSound *>   SoundVector;
   typedef SoundVector::iterator  SoundVectorItr;

   // How many sound we have
   int                  nextSoundInstanceIndex;
   //Audio System
   FMOD::System         *mFMODSystem;
   // Previous position of the system. For 3D Audio.
   Ogre::Vector3        mPrevListenerPosition;
   // Sounds vector
   SoundVector			*mSoundVector;
   // Channels vector
   iceChannel		    mChannelArray[MAX_SOUND_CHANNELS];

   };

#endif // __SOUNDMANAGER_H__