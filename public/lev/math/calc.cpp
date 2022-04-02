#include <lev/math/calc.h>

#include <math.h>

using namespace lev;

float Calc::abs(float x)
{
	return ::fabsf(x);
}

float Calc::mod(float x, float y)
{
	return ::fmodf(x, y);
}

float Calc::sqrt(float x)
{
	return ::sqrtf(x);
}

float Calc::invsqrt(float x)
{
	return 1.0f / sqrt(x);
}

float Calc::pow(float x, float e)
{
	return ::powf(x, e);
}

float Calc::exp(float x)
{
	return ::powf(Calc::E, x);
}

float Calc::sigmoid(float x)
{
	return 1.0f - (1.0f / (1.0f + Calc::exp(x)));
}

float Calc::log(float x, float b)
{
	return ::logf(x) / ::logf(b);
}

float Calc::log2(float x)
{
	return ::log2f(x);
}

float Calc::log10(float x)
{
	return ::log10f(x);
}

float Calc::ln(float x)
{
	return ::logf(x);
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
	return Calc::max(mn, Calc::min(mx, v));
}

float Calc::sign(float x)
{
	if (x < 0.0f)
		return -1.0f;
	else if (x > 0.0f)
		return 1.0f;

	return 0.0f;
}

float Calc::round(float x)
{
	return ::roundf(x);
}

float Calc::floor(float x)
{
	return ::floorf(x);
}

float Calc::ceil(float x)
{
	return ::ceilf(x);
}

bool Calc::within_epsilon(float lhs, float rhs, float epsilon)
{
	float delta = rhs - lhs;
	return ::fabsf(delta) <= epsilon;
}

float Calc::approach(float from, float to, float amount)
{
	return (to > from) ? Calc::min(from + amount, to) : Calc::max(from - amount, to);
}

float Calc::lerp(float from, float to, float amount)
{
	return from + ((to - from) * amount);
}

float Calc::spring(float from, float to, float bounciness, float tension, float& intermediate)
{
	intermediate = Calc::lerp(intermediate, (to - from) * tension, 1.0f / bounciness);
	return intermediate;
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
