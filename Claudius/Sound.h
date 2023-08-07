#pragma once

struct Sound
{
	Sound();
	Sound(const Sound& rhs);

	unsigned int id;
	unsigned int GetID();
};