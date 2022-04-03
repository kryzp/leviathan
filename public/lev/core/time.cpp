#include <lev/core/util.h>

using namespace lev;

float Time::milliseconds = 0.0f;
float Time::prev_milliseconds = 0.0f;
float Time::elapsed = 0.0f;
float Time::prev_elapsed = 0.0f;
float Time::delta = 0.0f;
u64 Time::loops = 0;
