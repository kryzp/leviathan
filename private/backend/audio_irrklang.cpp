#if LEV_USE_IRRKLANG

#include <backend/audio.h>

using namespace lv;
using namespace lv::bknd;

class IKSound : public Sound
{
public:
	IKSound(const char* path)
		: Sound()
	{
	}

	void play() override
	{
	}
};

class IKAudio : public Audio
{
public:
	AudioProperties properties() override
	{
		AudioProperties p = {0};
		p.temp = 3;

		return p;
	}

	bool init() override
	{
		return true;
	}

	void destroy() override
	{
	}

	void update() override
	{
	}

	Ref<Sound> create_sound(const char* path) override
	{
		return create_ref<IKSound>(path);
	}
};

Audio* Audio::inst()
{
	static IKAudio* instance = nullptr;
	if (!instance) { instance = new IKAudio(); }
	return instance;
}

#endif
