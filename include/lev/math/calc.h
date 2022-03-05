#pragma once

namespace lev
{
	namespace Calc
	{
		constexpr float E       = 2.71828182845f;
		constexpr float PI      = 3.14159265359f;
		constexpr float TAU     = 6.28318530718f;
		constexpr float RAD2DEG = 180.0f / PI;
		constexpr float DEG2RAD = PI / 180.0f;

		float abs(float x);
		float sqrt(float x);
		float invsqrt(float x);

		float pow(float x, float e);
		float exp(float x);
		float sigmoid(float x);

		float max(float a, float b);
		float min(float a, float b);
		float clamp(float v, float mn, float mx);

		float floor(float x);
		float ceil(float x);

		float approach(float from, float to, float amount);
		float lerp(float from, float to, float amount);

		float sin(float x);
		float cos(float x);
		float tan(float x);
		
		float asin(float x);
		float acos(float x);
		float atan(float x);
		float atan2(float y, float x);
	}
}
