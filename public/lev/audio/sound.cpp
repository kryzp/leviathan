#include <lev/audio/sound.h>
#include <backend/audio.h>

using namespace lv;

Ref<Sound> create(const char* path)
{
	return bknd::Audio::inst()->create_sound(path);
}
