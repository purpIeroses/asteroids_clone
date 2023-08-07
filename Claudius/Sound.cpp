#include "Sound.h"

Sound::Sound() : id(-1)
{
}

Sound::Sound(const Sound& rhs) : id(rhs.id)
{
}

unsigned int Sound::GetID()
{
	return id;
}
