#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "SoundManager.h"
#include "Sound.h"
#include "Music.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Sprite.h"
#include "Image.h"
#include "Transform.h"
#include "Font.h"

#include "Game.h"

#undef main

#pragma region ResourceManager
// ResourceManager 

struct ResourceImpl
{
	struct TextureResource
	{
		SDL_Texture* texture;
		int width;
		int height;
	};

	struct SoundResource
	{
		Mix_Chunk* chunk;
	};

	struct MusicResource
	{
		Mix_Music* music;
	};

	struct FontResource
	{
		TTF_Font* font;
		unsigned int size;
	};
	
	ResourceImpl() {};
	~ResourceImpl()
	{

		for (auto&& entry : textures)
		{
			SDL_DestroyTexture(entry.second.texture);
		}
		textures.clear();
		for (auto&& entry : sounds)
		{
			Mix_FreeChunk(entry.second.chunk);
		}
		sounds.clear();
		for (auto&& entry : musics)
		{
			Mix_FreeMusic(entry.second.music);
		}
		musics.clear();
		for (auto&& entry : fonts)
		{
			TTF_CloseFont(entry.second.font);
		}
		fonts.clear();
	}
	std::vector<std::pair<std::string, TextureResource>> textures;
	std::vector <std::pair<std::string, SoundResource>> sounds;
	std::vector <std::pair<std::string,  MusicResource>> musics;
	std::vector <std::pair<std::string, FontResource>> fonts;
	SDL_Renderer* renderer;

	ResourceImpl(SDL_Renderer* renderer) : renderer(renderer), lastError("") {};
	
	SDL_Texture* GetTexture(unsigned int id)
	{
		if (id < textures.size())
			return textures[id].second.texture;
		else
			return nullptr;
	}
	Mix_Chunk* GetChunk(unsigned int id)
	{
		if (id < sounds.size())
			return sounds[id].second.chunk;
		else
			return nullptr;
	}
	Mix_Music* GetMusic(unsigned int id)
	{
		if (id < musics.size())
			return musics[id].second.music;
		else
			return nullptr;
	}
	TTF_Font* GetFont(unsigned int id)
	{
		if (id < fonts.size())
			return fonts[id].second.font;
		else
			return nullptr;
	}
	

	const char* lastError;
};

ResourceManager::ResourceManager(ResourceImpl &resourceImpl) : impl(resourceImpl)
{

}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::LoadImageFromFile(Image& image, const std::string &filePath)
{
	impl.lastError = "";
	auto it = impl.textures.begin();
	unsigned int index = 0;
	while (it != impl.textures.end())
	{
		if ((*it).first == filePath)
		{
			image.id = index;
			image.width = (*it).second.width;
			image.height = (*it).second.height;
			return true;
		}
		it++;
		index++;
	}
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (surface != nullptr)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(impl.renderer, surface);
		if (texture != nullptr)
		{
			impl.textures.push_back(std::pair<std::string, ResourceImpl::TextureResource>(filePath, { texture,surface->w,surface->h } ));
			image.id = static_cast<unsigned int>(impl.textures.size() - 1);
			image.width = surface->w;
			image.height = surface->h;
			return true;
		}
		else
		{
			impl.lastError = SDL_GetError();
			return false;
		}
	}
	else
	{
		impl.lastError = IMG_GetError();
		return false;
	}
	return false;
};

bool ResourceManager::LoadSoundFromFile(Sound& sound, const std::string& filePath)
{
	impl.lastError = "";
	auto it = impl.sounds.begin();
	unsigned int index = 0;
	while (it != impl.sounds.end())
	{
		if ((*it).first == filePath)
		{
			sound.id = index;
			return true;
		}
		it++;
		index++;
	}
	Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
	if (chunk != nullptr)
	{
		impl.sounds.push_back(std::pair<std::string, ResourceImpl::SoundResource>(filePath, { chunk }));
		sound.id = static_cast<unsigned int>(impl.sounds.size() - 1);
		return true;
	}
	else
	{
		impl.lastError = Mix_GetError();
		return false;
	}
	return false;
};

bool ResourceManager::LoadMusicFromFile(Music& music, const std::string& filePath)
{
	impl.lastError = "";
	auto it = impl.musics.begin();
	unsigned int index = 0;
	while (it != impl.musics.end())
	{
		if ((*it).first == filePath)
		{
			music.id = index;
			return true;
		}
		it++;
		index++;
	}
	Mix_Music* chunk = Mix_LoadMUS(filePath.c_str());
	if (chunk != nullptr)
	{
		impl.musics.push_back(std::pair<std::string, ResourceImpl::MusicResource>(filePath, { chunk }));
		music.id = static_cast<unsigned int>(impl.musics.size() - 1);
		return true;
	}
	else
	{
		impl.lastError = Mix_GetError();
		return false;
	}
	return false;
};

bool ResourceManager::LoadFontFromFile(Font& font, const std::string& filePath, const unsigned int size)
{
	auto it = impl.fonts.begin();
	unsigned int index = 0;
	while (it != impl.fonts.end())
	{
		if ((*it).first == filePath)
		{
			if ((*it).second.size == size)
			{
				font.id = index;
				return true;
			}
		}
		it++;
		index++;
	}
	TTF_Font* f = TTF_OpenFont(filePath.c_str(), size);
	if (f != nullptr)
	{
		impl.fonts.push_back(std::pair<std::string, ResourceImpl::FontResource>(filePath, { f,size }));
		font.id = static_cast<unsigned int>(impl.fonts.size() - 1);
		return true;
	}
	else
	{
		impl.lastError = TTF_GetError();
		return false;
	}
	
	
}

const char* ResourceManager::GetError() 
{ 
	return impl.lastError;
}

#pragma endregion


#pragma region SoundManager
// SoundManager

enum class SoundCommand
{
	Play,
	Volume,
};

enum class MusicCommand
{
	Play,
	Stop,
	Volume
};

struct SoundEntry
{
	const Sound& sound;
	const int value;
	const SoundCommand command;
};

struct MusicEntry
{
	const Music& music;
	const int value;
	const MusicCommand command;
};
 
struct SoundImpl
{
	std::vector<SoundEntry> soundEntries;
	std::vector<MusicEntry> musicEntries;
	void Clear()
	{
		soundEntries.clear();
		musicEntries.clear();
	}
};

SoundManager::SoundManager(SoundImpl& soundImpl) : impl(soundImpl)
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::PlaySound(const Sound& sound, int loops)
{
	impl.soundEntries.push_back({ sound,loops, SoundCommand::Play});
}

void SoundManager::SetSoundVolume(const Sound& sound, int volume)
{
	impl.soundEntries.push_back({ sound,volume, SoundCommand::Volume });
}

void SoundManager::PlayMusic(const Music &music, int loops)
{
	impl.musicEntries.push_back({ music,loops,MusicCommand::Play });
}

void SoundManager::StopMusic(const Music &music)
{
	impl.musicEntries.push_back({ music,0,MusicCommand::Stop });
}

void SoundManager::SetMusicVolume(const Music& music, int volume)
{
	impl.musicEntries.push_back({ music,volume,MusicCommand::Volume });
}
#pragma endregion


#pragma region RenderManager

// Sprite, Rect and Text entries are used by the RenderManager to gather everything to be rendered
// before actually rendering them. These are references which means if adresses to local variables
// are being sent to the RenderManager they will not be rendered correctly since those temporary

struct SpriteEntry
{
	const Sprite& sprite;
	const Transform& transform;
};

struct RectEntry
{
	const Rectangle& rect;
	const Color& color;
};

struct TextEntry
{
	const Font& font;
	const Text& text;
};

struct RenderImpl
{
	SDL_Renderer* renderer = nullptr;
	std::vector<SpriteEntry> spriteEntries;
	std::vector<RectEntry> rectEntries;
	std::vector<TextEntry> textEntries;
	void Clear()
	{
		spriteEntries.clear();
		rectEntries.clear();
		textEntries.clear();
	}
};

RenderManager::RenderManager(RenderImpl& renderImpl) : impl(renderImpl)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Render(const Sprite& sprite, const Transform& transform)
{
	impl.spriteEntries.push_back({ sprite, transform });
}

void RenderManager::Render(const Rectangle& rect, const Color& color)
{
	impl.rectEntries.push_back({ rect, color });
}

void RenderManager::Render(const Font& font, const Text& text)
{
	impl.textEntries.push_back({ font, text });
}

#pragma endregion


#pragma region EnumTranslations

SDL_RendererFlip TranslateFlip(Sprite::Flip flip)
{
	switch (flip)
	{
	case Sprite::Flip::None: return SDL_RendererFlip::SDL_FLIP_NONE; break;
	case Sprite::Flip::Horizontal: return SDL_RendererFlip::SDL_FLIP_HORIZONTAL; break;
	case Sprite::Flip::Vertical: return SDL_RendererFlip::SDL_FLIP_VERTICAL; break;
	default: return SDL_RendererFlip::SDL_FLIP_NONE; break;
	}
}

KeyCode TranslateKeyCode(SDL_Keycode code)
{
	switch (code)
	{
	case SDLK_ESCAPE: return KeyCode::ESCAPE; break;
	case SDLK_SPACE: return KeyCode::SPACE; break;
	case SDLK_RETURN: return KeyCode::ENTER; break;
	case SDLK_RETURN2: return KeyCode::ENTER; break;
	case SDLK_a: return KeyCode::A; break;
	case SDLK_b: return KeyCode::B; break;
	case SDLK_c: return KeyCode::C; break;
	case SDLK_d: return KeyCode::D; break;
	case SDLK_e: return KeyCode::E; break;
	case SDLK_f: return KeyCode::F; break;
	case SDLK_g: return KeyCode::G; break;
	case SDLK_h: return KeyCode::H; break;
	case SDLK_i: return KeyCode::I; break;
	case SDLK_j: return KeyCode::J; break;
	case SDLK_k: return KeyCode::K; break;
	case SDLK_l: return KeyCode::L; break;
	case SDLK_m: return KeyCode::M; break;
	case SDLK_n: return KeyCode::N; break;
	case SDLK_o: return KeyCode::O; break;
	case SDLK_p: return KeyCode::P; break;
	case SDLK_q: return KeyCode::Q; break;
	case SDLK_r: return KeyCode::R; break;
	case SDLK_s: return KeyCode::S; break;
	case SDLK_t: return KeyCode::T; break;
	case SDLK_u: return KeyCode::U; break;
	case SDLK_v: return KeyCode::V; break;
	case SDLK_w: return KeyCode::W; break;
	case SDLK_x: return KeyCode::X; break;
	case SDLK_y: return KeyCode::Y; break;
	case SDLK_z: return KeyCode::Z; break;
	case SDLK_LEFT: return KeyCode::LEFT_ARROW; break;
	case SDLK_RIGHT: return KeyCode::RIGHT_ARROW; break;
	case SDLK_UP: return KeyCode::UP_ARROW; break;
	case SDLK_DOWN: return KeyCode::DOWN_ARROW; break;
	default: return KeyCode::INVALID; break;
	}
}

#pragma endregion


int main()
{

	#pragma region LibraryInitialization
	SDL_Init(SDL_INIT_EVERYTHING);
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		const char* error = IMG_GetError();
		return 0;
	}
	if (Mix_Init(MIX_INIT_OGG) == 0)
	{
		const char* error = Mix_GetError();
		return 0;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		const char* error = Mix_GetError();
		return 0;
	}
	if (TTF_Init() == -1)
	{
		const char* error = TTF_GetError();
		return 0;
	}
	SDL_Window* window = SDL_CreateWindow("Base", 0, 0, 0, 0, SDL_WindowFlags::SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		const char* error = SDL_GetError();
		return 0;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		const char* error = SDL_GetError();
		return 0;
	}
#pragma endregion 

	{
		bool running = true;
		bool quit = false;

		ResourceImpl resourceImpl(renderer);
		ResourceManager resourceManager(resourceImpl);

		RenderImpl renderImpl;
		RenderManager renderManager(renderImpl);

		SoundImpl soundImpl;
		SoundManager soundManager(soundImpl);

		Game game(resourceManager, soundManager);

		int width = 500;
		int height = 500;
		std::string title = "";
		game.Enter(width, height, title);
		SDL_SetWindowSize(window, width, height);
		SDL_SetWindowTitle(window, title.c_str());
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		unsigned int FPS = 60;
		unsigned int ticksPerFrame = 1000 / FPS;
		float dt = 0.16f;
		unsigned int processTick = 0;
		unsigned int previousTick = SDL_GetTicks();
		unsigned int delayTicks = 0;

		while (running && !quit)
		{
			unsigned int currentTick = SDL_GetTicks();
			unsigned int elapsedTicks = currentTick - previousTick;
			previousTick = currentTick;
			if (elapsedTicks < ticksPerFrame)
			{
				delayTicks = ticksPerFrame - elapsedTicks;
				SDL_Delay(delayTicks);
				dt = (elapsedTicks + delayTicks) * 0.001f;
			}
			else
				dt = elapsedTicks * 0.001f;

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_QUIT: quit = true; break;
				case SDL_KEYDOWN: game.OnKeyDown(TranslateKeyCode(e.key.keysym.sym)); break;
				case SDL_KEYUP: game.OnKeyUp(TranslateKeyCode(e.key.keysym.sym)); break;
				}
			}

			running = game.Update(dt);
			game.Render(renderManager);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			for (auto&& entry : renderManager.impl.spriteEntries) // Rendering the sprite entries
			{
				SDL_Rect src{ entry.sprite.source.x, entry.sprite.source.y, entry.sprite.source.w, entry.sprite.source.h };
				SDL_Rect dst{ static_cast<int>(entry.transform.position.x),
								static_cast<int>(entry.transform.position.y),
								static_cast<int>(entry.sprite.source.w * entry.transform.scale.x),
								static_cast<int>(entry.sprite.source.h * entry.transform.scale.y) };
				SDL_RenderCopyEx(renderer, resourceImpl.GetTexture(entry.sprite.image->id),
					&src, &dst, entry.transform.rotation, NULL, TranslateFlip(entry.sprite.flip));
			}
			for (auto&& entry : renderManager.impl.rectEntries) // Rendering the rectangle entries
			{
				SDL_SetRenderDrawColor(renderer, entry.color.r, entry.color.g, entry.color.b, entry.color.a);
				SDL_Rect rect{ entry.rect.x,
							   entry.rect.y,
							   entry.rect.w,
							   entry.rect.h };

				SDL_RenderDrawRect(renderer, &rect);
			}
			std::vector<SDL_Texture*> tmp;
			for (auto&& entry : renderManager.impl.textEntries) // Rendering the text entries
			{
				TTF_Font* f = resourceImpl.GetFont(entry.font.id);
				SDL_Surface* s = TTF_RenderText_Solid(f, entry.text.text.c_str(), SDL_Color{ entry.text.color.r,entry.text.color.g,entry.text.color.b,entry.text.color.a });
				if (s != nullptr)
				{
					SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
					SDL_FreeSurface(s);
					tmp.push_back(t);
					SDL_Rect src{ 0,0,s->w,s->h };
					SDL_Rect dst{ static_cast<int>(entry.text.transform.position.x),static_cast<int>(entry.text.transform.position.y),s->w,s->h };
					SDL_RenderCopyEx(renderer, t, &src, &dst, entry.text.transform.rotation, NULL, SDL_FLIP_NONE);
				}
			}
			for (auto&& entry : soundManager.impl.soundEntries) // Playing all sound entries
			{
				switch (entry.command)
				{
				case SoundCommand::Play: Mix_PlayChannel(-1, resourceImpl.GetChunk(entry.sound.id), entry.value); break;
				case SoundCommand::Volume: Mix_VolumeChunk(resourceImpl.GetChunk(entry.sound.id), entry.value); break;
				}
			}
			for (auto&& entry : soundManager.impl.musicEntries) // Playing all sound entries
			{
				switch (entry.command)
				{
				case MusicCommand::Play: Mix_PlayMusic(resourceImpl.GetMusic(entry.music.id), entry.value); break;
				case MusicCommand::Stop: Mix_HaltMusic(); break;
				case MusicCommand::Volume: Mix_VolumeMusic(entry.value); break;
				}
			}
			SDL_RenderPresent(renderer);
			for (SDL_Texture* t : tmp)
				SDL_DestroyTexture(t);

			tmp.clear();
			soundManager.impl.Clear();
			renderManager.impl.Clear();
			processTick = (SDL_GetTicks() - currentTick);
		}
	}

	#pragma region LibraryCleanup

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	#pragma endregion

	return 0;
}