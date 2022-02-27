#include <lev/math/ease.h>
#include <lev/math/calc.h>

// thanks easings.net!

using namespace Lev;

float Ease::linear(float t)
{
	return t;
}

float Ease::elastic_in(float t)
{
	return Calc::sin(13 * Calc::PI * 0.5f * t) * Calc::pow(2, 10 * (t-1));
}

float Ease::elastic_out(float t)
{
	return Calc::sin(-13 * Calc::PI * 0.5f * (t+1)) * Calc::pow(2, -10*t) + 1;
}

float Ease::elastic_in_out(float t)
{
	if (t < 0.5f)
		return 0.5f * Calc::sin(13 * Calc::PI * 0.5f * 2*t) * Calc::pow(2, 10 * (2*t - 1));

	return 0.5f * (Calc::sin(-13 * Calc::PI * 0.5f * ((2*t - 1) + 1)) * Calc::pow(2, -10 * (2*t - 1)) + 2);
}

float Ease::quadratic_in(float t)
{
	return t*t;
}

float Ease::quadratic_out(float t)
{
	return -t * (t-2);
}

float Ease::quadratic_in_out(float t)
{
	if (t < 0.5f)
		return 2*t*t;

	return 1 - 2*t*t;
}

float Ease::sine_in(float t)
{
	return -Calc::cos(t * Calc::PI * 0.5f) + 1;
}

float Ease::sine_out(float t)
{
	return Calc::sin(t * Calc::PI * 0.5f);
}

float Ease::sine_in_out(float t)
{
	return -Calc::cos(t * Calc::PI) * 0.5f + 0.5f;
}

float Ease::exp_in(float t)
{
	return Calc::pow(2, 10*(t-1));
}

float Ease::exp_out(float t)
{
	return -Calc::pow(2, -10*t) + 1;
}

float Ease::exp_in_out(float t)
{
	if (t < 0.5f)
		return Calc::pow(2, 10 * (2*t - 1)) * 0.5f;

	return -(Calc::pow(2, -10 * (2*t - 1)) + 2) * 0.5f;
}

float Ease::back_in(float t)
{
	return 2.70158*t*t*t - 1.70158*t*t;
}

float Ease::back_out(float t)
{
	return 1 + (2.70158f * Calc::pow(t-1, 3) + 1.70158f * Calc::pow(t - 1, 2));
}

float Ease::back_in_out(float t)
{
	if (t < 0.5f)
		return (Calc::pow(2*t, 2) * (3.5949095f*2*t - 2.5949095f)) * 0.5f;

	return (Calc::pow(2*t - 2, 2) * (3.5949095f*(2*t - 2) + 2.5949095f) + 2) * 0.5f;
}

float Ease::circ_in(float t)
{
	return 1.0f - Calc::sqrt(1.0f - Calc::pow(t, 2));
}

float Ease::circ_out(float t)
{
	return Calc::sqrt(1.0f - Calc::pow(t - 1, 2));
}

float Ease::circ_in_out(float t)
{
	if (t < 0.5f)
		return (1.0f - Calc::sqrt(1.0f - Calc::pow(2*t, 2))) * 0.5f;

	return (Calc::sqrt(1 - Calc::pow(-2*t + 2, 2)) + 1) * 0.5f;
}
