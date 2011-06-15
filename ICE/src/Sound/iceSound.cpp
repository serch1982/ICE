#include <Sound\iceSound.h>

void iceSound::Initialize(void)
{
   fileName.clear();
   fmodSound = NULL;
   soundType = SOUND_TYPE_INVALID;
}

void iceSound::Finalize(void)
{
	fileName.clear();
	fmodSound->release();
	soundType = SOUND_TYPE_INVALID;
}
