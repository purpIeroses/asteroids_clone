#include "Music.h"

Music::Music() : id(-1)
{
}

Music::Music(const Music& rhs) : id(rhs.id)
{
}

unsigned int Music::GetID()
{
	return id;
}
