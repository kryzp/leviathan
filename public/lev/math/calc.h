#pragma once

#include <limits>

namespace lv
{
	namespace calc
	{
		constexpr float E       = 2.71828182845f;
		constexpr float PI      = 3.14159265359f;
		constexpr float TAU     = 6.28318530718f;
		constexpr float RAD2DEG = 180.0f / PI;
		constexpr float DEG2RAD = PI / 180.0f;

		bool is_prime(unsigned x);

		float abs(float x);
		float mod(float x, float y);
		float sqrt(float x);
		float invsqrt(float x);

		float pow(float x, float e);
		float exp(float x);
		float sigmoid(float x);
		float sign(float x);
		float snap(float x, float interval);

		float log(float x, float b);
		float log2(float x);
		float log10(float x);
		float ln(float x);

		float max(float a, float b);
		float min(float a, float b);
		float clamp(float v, float mn, float mx);

		float round(float x);
		float floor(float x);
		float ceil(float x);

		bool within_epsilon(float lhs, float rhs, float epsilon = std::numeric_limits<float>::epsilon());

		float approach(float from, float to, float amount);
		float lerp(float from, float to, float amount);
		float lerp_t(float from, float to, float amount, float t);
		float spring(float from, float to, float bounciness, float tension, float& intermediate);

		float sin(float x);
		float cos(float x);
		float tan(float x);

		float sinh(float x);
		float cosh(float x);
		float tanh(float x);
		
		float asin(float x);
		float acos(float x);
		float atan(float x);
		float atan2(float y, float x);
	}
}
