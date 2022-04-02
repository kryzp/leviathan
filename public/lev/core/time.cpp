#include <lev/core/util.h>

using namespace lev;

float Time::seconds = 0.0f;
float Time::prev_seconds = 0.0f;
float Time::milliseconds = 0.0f;
float Time::prev_milliseconds = 0.0f;
float Time::delta = 0.0f;
u64 Time::frames = 0;
