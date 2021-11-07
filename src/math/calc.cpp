#include <lev/math/calc.h>

#include <math.h>

using namespace LEV;

float Calc::abs(float x)
{
	return ::fabsf(x);
}

float Calc::sqrt(float x)
{
	return ::sqrtf(x);
}

float Calc::invsqrt(float x)
{
	return 1.0f / sqrt(x);
}

float Calc::max(float a, float b)
{
	return (a > b) ? a : b;
}

float Calc::min(float a, float b)
{
	return (a < b) ? a : b;
}

float Calc::clamp(float v, float mn, float mx)
{
	return max(mn, min(mx, v));
}

float Calc::approach(float from, float to, float amount)
{
	return (to > from) ? min(from + amount, to) : max(from - amount, to);
}

float Calc::lerp(float from, float to, float amount)
{
	return from + ((to - from) * amount);
}

float Calc::sin(float x)
{
	return ::sinf(x);
}

float Calc::cos(float x)
{
	return ::cosf(x);
}

float Calc::tan(float x)
{
	return ::tanf(x);
}

float Calc::asin(float x)
{
	return ::asinf(x);
}

float Calc::acos(float x)
{
	return ::acosf(x);
}

float Calc::atan(float x)
{
	return ::atanf(x);
}

float Calc::atan2(float y, float x)
{
	return ::atan2f(y, x);
}
