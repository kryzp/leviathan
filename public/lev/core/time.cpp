#include <lev/core/util.h>

using namespace lev;

float time::milliseconds = 0.0f;
float time::prev_milliseconds = 0.0f;
float time::elapsed = 0.0f;
float time::prev_elapsed = 0.0f;
float time::delta = 0.0f;
u64 time::step_count = 0;
u64 time::run_loops = 0;
