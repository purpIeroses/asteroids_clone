#pragma once

#include <string>

struct Image;
struct Sound;
struct Music;
struct Font;
struct ResourceImpl;

struct ResourceManager
{
	ResourceManager(ResourceImpl& impl);
	~ResourceManager();
	
	bool LoadImageFromFile(Image& image, const std::string &filePath);
	bool LoadSoundFromFile(Sound& sound, const std::string &filePath);
	bool LoadMusicFromFile(Music& music, const std::string& filePath);
	bool LoadFontFromFile(Font& font, const std::string& filePath, const unsigned int size);

	const char* GetError();
	
	ResourceImpl& impl;
};
