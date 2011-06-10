#include <Sound\iceSoundManager.h>
#include <fmod_errors.h>
#include <OgreException.h>

#define INITIAL_VECTOR_SIZE   100
#define INCREASE_VECTOR_SIZE  20

#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5

#define MAX_SOUNDS			150
/*#define
#define
#define
#define
#define*/

template<> iceSoundManager* Ogre::Singleton<iceSoundManager>::ms_Singleton = 0;

iceSoundManager::iceSoundManager()
{
   FMOD::System_Create( &mFMODSystem );
   mPrevListenerPosition = Ogre::Vector3(0, 0, 0);
   mSoundVector = new SoundVector;

   //soundVector has MAX_SOUNDS sounds we designed.
   mSoundVector->resize(MAX_SOUNDS);
   for (int vectorIndex = 0; vectorIndex < INITIAL_VECTOR_SIZE; vectorIndex++)
      {
      mSoundVector->at(vectorIndex) = new iceSound;
      mSoundVector->at(vectorIndex)->Clear();
      }

   for (int channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
      mChannelArray[channelIndex].Clear();
}


iceSoundManager::~iceSoundManager()
   {
   for (int vectorIndex = 0; vectorIndex < (int)mSoundVector->capacity(); vectorIndex++)
      {
      mSoundVector->at(vectorIndex)->fileName.clear();
//      soundInstanceVector->at(vectorIndex)->streamPtr->close();
      delete mSoundVector->at(vectorIndex);
      }

   if(mFMODSystem)
      mFMODSystem->release();
   }


void iceSoundManager::Initialize(void)
   {
   FMOD_RESULT result;

   // Create the main system object.
   result = FMOD::System_Create(&mFMODSystem);
   if (result != FMOD_OK)
      OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

   result = mFMODSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
   if (result != FMOD_OK)
      OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

   mFMODSystem->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);

   //result = system->setFileSystem(&fmodFileOpenCallback, &fmodFileCloseCallback, &fmodFileReadCallback, &fmodFileSeekCallback, 2048);
   //if (result != FMOD_OK)
   //   OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

   Ogre::LogManager::getSingleton().logMessage("SoundManager Initialized");
   }


iceSoundManager* iceSoundManager::getSingletonPtr(void)
   {
   return ms_Singleton;
   }


iceSoundManager& iceSoundManager::getSingleton(void)
   {  
   assert( ms_Singleton );  return ( *ms_Singleton );  
   }


void iceSoundManager::FrameStarted(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed)
   {
   int            channelIndex;
   FMOD::Channel *nextChannel;
   FMOD_VECTOR    listenerPosition;
   FMOD_VECTOR    listenerForward;
   FMOD_VECTOR    listenerUp;
   FMOD_VECTOR    listenerVelocity;
   Ogre::Vector3  vectorVelocity;
   Ogre::Vector3  vectorForward;
   Ogre::Vector3  vectorUp;
   Ogre::Vector3  vectorX;
   Ogre::Quaternion quaternion;

   if (timeElapsed > 0)
	   vectorVelocity = (listenerNode->convertLocalToWorldPosition( listenerNode->getPosition()) - mPrevListenerPosition) / timeElapsed;
   else
      vectorVelocity = Ogre::Vector3(0, 0, 0);

   listenerNode->convertLocalToWorldOrientation( listenerNode->getOrientation() );
   quaternion.ToAxes( vectorX, vectorUp, vectorForward );

   //vectorForward = listenerNode->convertLocalToWorldOrientation( listenerNode->getOrientation() ).ToAxes( NULL, vectorUp, vectorForward ));
   vectorForward.normalise();

   //vectorUp = listenerNode->getWorldOrientation().yAxis();
   vectorUp.normalise();

   listenerPosition.x = listenerNode->convertLocalToWorldPosition( listenerNode->getPosition() ).x;
   listenerPosition.y = listenerNode->convertLocalToWorldPosition( listenerNode->getPosition() ).y;
   listenerPosition.z = listenerNode->convertLocalToWorldPosition( listenerNode->getPosition() ).z;

   listenerForward.x = vectorForward.x;
   listenerForward.y = vectorForward.y;
   listenerForward.z = vectorForward.z;

   listenerUp.x = vectorUp.x;
   listenerUp.y = vectorUp.y;
   listenerUp.z = vectorUp.z;

   listenerVelocity.x = vectorVelocity.x;
   listenerVelocity.y = vectorVelocity.y;
   listenerVelocity.z = vectorVelocity.z;

   // update 'ears'
   mFMODSystem->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &listenerForward, &listenerUp);
   mFMODSystem->update();

   mPrevListenerPosition = listenerNode->convertLocalToWorldPosition( listenerNode->getPosition() );

   for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
      {
      if (mChannelArray[channelIndex].sceneNode != NULL)
         {
         mFMODSystem->getChannel(channelIndex, &nextChannel);
         if (timeElapsed > 0)
            vectorVelocity = (mChannelArray[channelIndex].sceneNode->convertLocalToWorldPosition( listenerNode->getPosition() ) - mChannelArray[channelIndex].prevPosition) / timeElapsed;
         else
            vectorVelocity = Ogre::Vector3(0, 0, 0);

		 listenerPosition.x = mChannelArray[channelIndex].sceneNode->convertLocalToWorldPosition( listenerNode->getPosition() ).x;
         listenerPosition.y = mChannelArray[channelIndex].sceneNode->convertLocalToWorldPosition( listenerNode->getPosition() ).y;
         listenerPosition.z = mChannelArray[channelIndex].sceneNode->convertLocalToWorldPosition( listenerNode->getPosition() ).z;

         listenerVelocity.x = vectorVelocity.x;
         listenerVelocity.y = vectorVelocity.y;
         listenerVelocity.z = vectorVelocity.z;

         nextChannel->set3DAttributes(&listenerPosition, &listenerVelocity);
         mChannelArray[channelIndex].prevPosition = mChannelArray[channelIndex].sceneNode->convertLocalToWorldPosition( listenerNode->getPosition() );
         }
      }
   }



int iceSoundManager::CreateStream(Ogre::String &fileName)
   {
   return CreateSound(fileName, SOUND_TYPE_2D_SOUND);
   }


int iceSoundManager::CreateSound(Ogre::String &fileName)
   {
   return CreateSound(fileName, SOUND_TYPE_3D_SOUND);
   }


int iceSoundManager::CreateLoopedSound(Ogre::String &fileName)
   {
   return CreateSound(fileName, SOUND_TYPE_3D_SOUND_LOOPED);
   }


int iceSoundManager::CreateLoopedStream(Ogre::String &fileName)
   {
   return CreateSound(fileName, SOUND_TYPE_2D_SOUND_LOOPED);
   }



// fileName is actually a pointer to a SoundInstance, passed in from CreateSound().
//FMOD_RESULT SoundManager::fmodFileOpenCallback(const char *fileName, int unicode, unsigned int *filesize, void **handle, void **userdata)
//   {
//   SoundInstance *soundInstance;
//
//   assert(fileName);
//
//   soundInstance = (SoundInstance *)fileName;
//   assert(soundInstance->fileArchive);
//
//   *handle = (void *)soundInstance;
//   *userdata = NULL;
//
//   soundInstance->streamPtr = soundInstance->fileArchive->open(soundInstance->fileName);
//   if (soundInstance->streamPtr.isNull())
//      {
//      *filesize = 0;
//      return FMOD_ERR_FILE_NOTFOUND;
//      }
//
//   *filesize = (unsigned int)soundInstance->streamPtr->size();
//   return FMOD_OK;
//   }
//
//
//FMOD_RESULT SoundManager::fmodFileCloseCallback(void *handle, void *userdata)
//   {
//   return FMOD_OK;
//   }
//
//
//FMOD_RESULT SoundManager::fmodFileReadCallback(void *handle, void *buffer, unsigned int sizeBytes, unsigned int *bytesRead, void *userData)
//   {
//   SoundInstance *soundInstance;
//
//   soundInstance = (SoundInstance *)handle;
//   *bytesRead = (unsigned int)soundInstance->streamPtr->read(buffer, (size_t)sizeBytes);
//   if (*bytesRead == 0)
//      return FMOD_ERR_FILE_EOF;
//
//   return FMOD_OK;
//   }
//
//
//FMOD_RESULT SoundManager::fmodFileSeekCallback(void *handle, unsigned int pos, void *userdata)
//   {
//   SoundInstance *soundInstance;
//
//   soundInstance = (SoundInstance *)handle;
//   soundInstance->streamPtr->seek((size_t)pos);
//   return FMOD_OK;
//   }
//
//

int iceSoundManager::CreateSound(Ogre::String &fileName, SOUND_TYPE soundType)
   {
   Ogre::Archive	*fileArchive;
   FMOD_RESULT		result;
   FMOD::Sound		*sound;
   Ogre::String		fullPathName;
   iceSound			*newSoundInstance;

   int soundIndex;
   soundIndex = FindSound(fileName, soundType);
   if (soundIndex != INVALID_SOUND_INDEX)
      return soundIndex;

   fullPathName = fileName;
   iceFileLocator * fileLocator = (iceFileLocator * )Ogre::ResourceGroupManager::getSingletonPtr();
   fileArchive = fileLocator->Find(fullPathName);
   if (!fileArchive)
      {
      Ogre::LogManager::getSingleton().logMessage("SoundManager::CreateSound could not find sound '" + fileName + "'");
      return INVALID_SOUND_INDEX;
      }

   newSoundInstance = mSoundVector->at(nextSoundInstanceIndex);
   newSoundInstance->fileName = fileName;
   newSoundInstance->soundType = soundType;

   switch (soundType)
      {
      case SOUND_TYPE_3D_SOUND:
         {
         result = mFMODSystem->createSound((const char *)newSoundInstance, FMOD_3D, 0, &sound);
         break;
         }

      case SOUND_TYPE_3D_SOUND_LOOPED:
         {
         result = mFMODSystem->createSound((const char *)newSoundInstance, FMOD_LOOP_NORMAL | FMOD_3D | FMOD_HARDWARE, 0, &sound);
         break;
         }

      case SOUND_TYPE_2D_SOUND:
         {
         result = mFMODSystem->createStream((const char *)newSoundInstance, FMOD_DEFAULT, 0, &sound);
         break;
         }

      case SOUND_TYPE_2D_SOUND_LOOPED:
         {
         result = mFMODSystem->createStream((const char *)newSoundInstance, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound);
         break;
         }

      default:
         {
         Ogre::LogManager::getSingleton().logMessage("SoundManager::CreateSound could not load sound '" + fileName + "' (invalid soundType)");
         return INVALID_SOUND_INDEX;
         }
      }

   if (result != FMOD_OK)
      {
      Ogre::LogManager::getSingleton().logMessage("SoundManager::CreateSound could not load sound '" + fileName + "'  FMOD Error:" + FMOD_ErrorString(result));
      return INVALID_SOUND_INDEX;
      }

   newSoundInstance->fmodSound = sound;
   return nextSoundInstanceIndex;
   }


void iceSoundManager::PlaySound(int soundIndex, Ogre::SceneNode *soundNode, int *channelIndex)
   {
   int            channelIndexTemp;
   FMOD_RESULT    result;
   FMOD_VECTOR    initialPosition;
   FMOD::Channel *channel;
   iceSound*	  sound;

   if (soundIndex == INVALID_SOUND_INDEX)
      return;

   if (channelIndex)
      channelIndexTemp = *channelIndex;
   else
      channelIndexTemp = INVALID_SOUND_CHANNEL;

   assert((soundIndex > 0) && (soundIndex < (int)mSoundVector->capacity()));

   // If the channelIndex already has a sound assigned to it, test if it's the same sceneNode.
   if ((channelIndexTemp != INVALID_SOUND_CHANNEL) && (mChannelArray[channelIndexTemp].sceneNode != NULL))
      {
      result = mFMODSystem->getChannel(channelIndexTemp, &channel);
      if (result == FMOD_OK)
         {
         bool isPlaying;

         result = channel->isPlaying(&isPlaying);
         if ((result == FMOD_OK) && (isPlaying == true) && (mChannelArray[channelIndexTemp].sceneNode == soundNode))
            return;  // Already playing this sound attached to this node.
         }
      }

   sound = mSoundVector->at(soundIndex);
   // Start the sound paused
   result = mFMODSystem->playSound(FMOD_CHANNEL_FREE, sound->fmodSound, true, &channel);
   if (result != FMOD_OK)
      {
      Ogre::LogManager::getSingleton().logMessage(Ogre::String("SoundManager::PlaySound could not play sound  FMOD Error:") + FMOD_ErrorString(result));
      if (channelIndex)
         *channelIndex = INVALID_SOUND_CHANNEL;
      return;
      }

   channel->getIndex(&channelIndexTemp);
   mChannelArray[channelIndexTemp].sceneNode = soundNode;

   if (soundNode)
      {
      mChannelArray[channelIndexTemp].prevPosition = soundNode->convertLocalToWorldPosition( soundNode->getPosition() );

      initialPosition.x = soundNode->convertLocalToWorldPosition( soundNode->getPosition() ).x;
      initialPosition.y = soundNode->convertLocalToWorldPosition( soundNode->getPosition() ).y;
      initialPosition.z = soundNode->convertLocalToWorldPosition( soundNode->getPosition() ).z;
      channel->set3DAttributes(&initialPosition, NULL);
      }

   result = channel->setVolume(1.0);
   // This is where the sound really starts.
   result = channel->setPaused(false);

   if (channelIndex)
      *channelIndex = channelIndexTemp;
   }



void iceSoundManager::Set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance)
   {
   FMOD_RESULT    result;
   FMOD::Channel *channel;

   if (channelIndex == INVALID_SOUND_CHANNEL)
      return;

   result = mFMODSystem->getChannel(channelIndex, &channel);
   if (result == FMOD_OK)
      channel->set3DMinMaxDistance(minDistance, maxDistance);
   }



void iceSoundManager::StopAllSounds(void)
   {
   int            channelIndex;
   FMOD_RESULT    result;
   FMOD::Channel *nextChannel;

   for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
      {
      result = mFMODSystem->getChannel(channelIndex, &nextChannel);
      if ((result == FMOD_OK) && (nextChannel != NULL))
         nextChannel->stop();
      mChannelArray[channelIndex].Clear();
      }
   }


void iceSoundManager::StopSound(int *channelIndex)
   {
   if (*channelIndex == INVALID_SOUND_CHANNEL)
      return;

   FMOD::Channel *soundChannel;

   assert((*channelIndex > 0) && (*channelIndex < MAX_SOUND_CHANNELS));

   mFMODSystem->getChannel(*channelIndex, &soundChannel);
   soundChannel->stop();

   mChannelArray[*channelIndex].Clear();
   *channelIndex = INVALID_SOUND_CHANNEL;
   }



int iceSoundManager::FindSound(Ogre::String &fileName, SOUND_TYPE soundType)
   {
   int            vectorIndex;
   int            vectorCapacity;
   iceSound		*nextSoundInstance;

   vectorCapacity = (int)mSoundVector->capacity();
   for (vectorIndex = 0; vectorIndex < vectorCapacity; vectorIndex++)
      {
      nextSoundInstance = mSoundVector->at(vectorIndex);
      if ((soundType == nextSoundInstance->soundType) && (fileName == nextSoundInstance->fileName))
//      if ((soundType == nextSoundInstance->soundType) && (fileName == nextSoundInstance->fileArchive->getName()))
         return vectorIndex;
      }

   return INVALID_SOUND_INDEX;
   }



float iceSoundManager::GetSoundLength(int soundIndex)
   {
   if (soundIndex == INVALID_SOUND_INDEX)
      return 0.0;

   assert((soundIndex > 0) && (soundIndex < (int)mSoundVector->capacity()));

   unsigned int	soundLength;   // length in milliseconds
   FMOD_RESULT	result;
   iceSound		*sound;

   sound = mSoundVector->at(soundIndex);
   if (sound)
      {
      result = sound->fmodSound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
      if (result != FMOD_OK)
         {
         Ogre::LogManager::getSingleton().logMessage(Ogre::String("SoundManager::GetSoundLength could not get length  FMOD Error:") + FMOD_ErrorString(result));
         return 0.0;
         }
      }
   else
      {
      Ogre::LogManager::getSingleton().logMessage(Ogre::String("SoundManager::GetSoundLength could not find soundInstance"));
      return 0.0;
      }

   return (float)soundLength / 1000.0f;
   }

