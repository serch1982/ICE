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
	//Create the FMOD System
	FMOD::System_Create( &mFMODSystem );
	//Initialize the prevPosition
	mPrevListenerPosition = Ogre::Vector3(0, 0, 0);

	//Create the sound vector
	mSoundVector = new SoundVector;
	//soundVector has MAX_SOUNDS sounds we designed.
	mSoundVector->resize(MAX_SOUNDS);

	//for each instance, init its parameters
	for (int vectorIndex = 0; vectorIndex < MAX_SOUNDS; vectorIndex++)
	{
		mSoundVector->at(vectorIndex) = new iceSound;
		mSoundVector->at(vectorIndex)->Initialize();
	}

	//Init all channels
	for (int channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
		mChannelArray[channelIndex].Clear();

	nextSoundInstanceIndex = 0;
}

// Destructor
iceSoundManager::~iceSoundManager()
{
	FMOD_RESULT result;

	// No more sounds playing
	StopAllSounds();

	for (int vectorIndex = 0; vectorIndex < (int)mSoundVector->capacity(); vectorIndex++)
	{
		mSoundVector->at(vectorIndex)->fileName.clear();
		// soundInstanceVector->at(vectorIndex)->streamPtr->close();
		delete mSoundVector->at(vectorIndex);
	}

	// Finish FMOD System
	if(mFMODSystem){
		result = mFMODSystem->close();
		if( result != FMOD_OK )
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
		result = mFMODSystem->release();
		if( result != FMOD_OK )
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
	}
}

// Function provided by the Getting Started Guide of FMOD
bool iceSoundManager::Initialize(void)
{
	FMOD_RESULT result;
	FMOD_CAPS		caps;
    char			name[256];
	unsigned int	version;
    FMOD_SPEAKERMODE speakermode;
	int				 numdrivers;

	/*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&mFMODSystem);
    if (result != FMOD_OK)
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
    
    result = mFMODSystem->getVersion(&version);
    if (result != FMOD_OK)
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return false;
    }
    
    result = mFMODSystem->getNumDrivers(&numdrivers);
    if (result != FMOD_OK)
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

    if (numdrivers == 0)
    {
        result = mFMODSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
    }
    else
    {
        result = mFMODSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

        result = mFMODSystem->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
            result = mFMODSystem->setDSPBufferSize(1024, 10);
            if (result != FMOD_OK)
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
        }

        result = mFMODSystem->getDriverInfo(0, name, 256, 0);
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

        if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
        {
            result = mFMODSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            if (result != FMOD_OK)
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
        }
    }

    result = mFMODSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = mFMODSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
            
        result = mFMODSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
        if (result != FMOD_OK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");
    }
		
	result = mFMODSystem->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);
	if (result != FMOD_OK)
	   OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + Ogre::StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

	//result = system->setFileSystem(&fmodFileOpenCallback, &fmodFileCloseCallback, &fmodFileReadCallback, &fmodFileSeekCallback, 2048);
	//if (result != FMOD_OK)
	//   OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "SoundManager::Initialize");

	Ogre::LogManager::getSingleton().logMessage("SoundManager Initialized");

	return true;
}

// Singleton
iceSoundManager* iceSoundManager::getSingletonPtr(void)
{
   return ms_Singleton;
}

iceSoundManager& iceSoundManager::getSingleton(void)
{  
	assert( ms_Singleton );
	return ( *ms_Singleton );  
}

// fake update
//void iceSoundManager::update( Ogre::Real timeElapsed ){
//	mFMODSystem->update();
//}

// true update
void iceSoundManager::update( Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Real timeElapsed )
//void iceSoundManager::FrameStarted(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed)
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
	Ogre::Quaternion quaternion;

	if (timeElapsed > 0)
	   vectorVelocity = (position - mPrevListenerPosition) / timeElapsed;
	else
	   vectorVelocity = Ogre::Vector3(0, 0, 0);

	vectorForward = quaternion.zAxis();
	vectorForward.normalise();

	vectorUp = quaternion.yAxis();
	vectorUp.normalise();

	listenerPosition.x = position.x;
	listenerPosition.y = position.y;
	listenerPosition.z = position.z;

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

	mPrevListenerPosition = position;

	for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
	{
		mFMODSystem->getChannel(channelIndex, &nextChannel);
		if (timeElapsed > 0)
			vectorVelocity = (mChannelArray[channelIndex].curPosition - mChannelArray[channelIndex].prevPosition) / timeElapsed;
		else
			vectorVelocity = Ogre::Vector3(0, 0, 0);

		listenerPosition.x = mChannelArray[channelIndex].curPosition.x;
		listenerPosition.y = mChannelArray[channelIndex].curPosition.y;
		listenerPosition.z = mChannelArray[channelIndex].curPosition.z;

		listenerVelocity.x = vectorVelocity.x;
		listenerVelocity.y = vectorVelocity.y;
		listenerVelocity.z = vectorVelocity.z;

		nextChannel->set3DAttributes(&listenerPosition, &listenerVelocity);
		mChannelArray[channelIndex].prevPosition = mChannelArray[channelIndex].curPosition;
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

void iceSoundManager::loadResourcesMenu(){
	CreateLoopedStream( Ogre::String( "menu.mp3" ));
	CreateLoopedStream( Ogre::String( "test.mp3" ));
	
}

void iceSoundManager::unloadResourcesMenu(){
	(*mSoundVector)[0]->Finalize();
}

// Load level 1 sounds
void iceSoundManager::loadLevel1(){
	CreateSound( Ogre::String( "level1.mp3" ), SOUND_TYPE_2D_SOUND_LOOPED, 0);
}

// UnLoad level 1 sounds
void iceSoundManager::unloadLevel1(){
	(*mSoundVector)[0]->Finalize();
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

int iceSoundManager::CreateSound(Ogre::String &fileName, SOUND_TYPE soundType, int p_sound_index)
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

	if( p_sound_index >= 0 )
		newSoundInstance = mSoundVector->at(p_sound_index);
	else
		newSoundInstance = mSoundVector->at(nextSoundInstanceIndex);
	newSoundInstance->fileName = fullPathName;
	newSoundInstance->soundType = soundType;

	switch (soundType)
	   {
	   case SOUND_TYPE_3D_SOUND:
		  {
		  result = mFMODSystem->createSound(newSoundInstance->fileName.c_str(), FMOD_3D, 0, &sound);
		  break;
		  }

	   case SOUND_TYPE_3D_SOUND_LOOPED:
		  {
		  result = mFMODSystem->createSound(newSoundInstance->fileName.c_str(), FMOD_LOOP_NORMAL | FMOD_3D | FMOD_HARDWARE, 0, &sound);
		  break;
		  }

	   case SOUND_TYPE_2D_SOUND:
		  {
		  result = mFMODSystem->createStream(newSoundInstance->fileName.c_str(), FMOD_DEFAULT, 0, &sound);
		  break;
		  }

	   case SOUND_TYPE_2D_SOUND_LOOPED:
		  {
		  result = mFMODSystem->createStream(newSoundInstance->fileName.c_str(), FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound);
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
	
	if( p_sound_index < 0 )
		nextSoundInstanceIndex++;
	return nextSoundInstanceIndex;
}

//void iceSoundManager::PlaySound(int soundIndex, Ogre::SceneNode *soundNode, int *channelIndex)

void iceSoundManager::PlaySound(int soundIndex, Ogre::Vector3 soundPosition, int *channelIndex)
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

   assert((soundIndex >= 0) && (soundIndex < (int)mSoundVector->capacity()));

   // If the channelIndex already has a sound assigned to it, test if it's the same sceneNode.
   if (channelIndexTemp != INVALID_SOUND_CHANNEL)
   {
      result = mFMODSystem->getChannel(channelIndexTemp, &channel);
      if (result == FMOD_OK)
         {
         bool isPlaying;

         result = channel->isPlaying(&isPlaying);
         if ((result == FMOD_OK) && (isPlaying == true))
            return;  // Already playing this sound attached to this node.
         }
   }

   sound = mSoundVector->at(soundIndex);
   // Start the sound paused
   result = mFMODSystem->playSound(FMOD_CHANNEL_FREE, sound->fmodSound, true, &channel);
   if (result != FMOD_OK){
      Ogre::LogManager::getSingleton().logMessage(Ogre::String("SoundManager::PlaySound could not play sound  FMOD Error:") + FMOD_ErrorString(result));
      if (channelIndex)
         *channelIndex = INVALID_SOUND_CHANNEL;
      return;
   }

   channel->getIndex(&channelIndexTemp);
   mChannelArray[channelIndexTemp].curPosition = soundPosition;

   //if (soundNode)
   //   {
		mChannelArray[channelIndexTemp].prevPosition = soundPosition;

		initialPosition.x = soundPosition.x;
		initialPosition.y = soundPosition.y;
		initialPosition.z = soundPosition.z;
		channel->set3DAttributes(&initialPosition, NULL);
   //   }

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

