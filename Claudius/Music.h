#pragma once

struct Music
{
	Music();
	Music(const Music& rhs);

	unsigned int id;
	unsigned int GetID();
};