#include <Sound\iceSound.h>

void iceSound::Clear(void)
{
   fileName.clear();
   fmodSound = NULL;
   soundType = SOUND_TYPE_INVALID;
}
