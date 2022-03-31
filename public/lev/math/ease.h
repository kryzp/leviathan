#pragma once

namespace lev
{
	namespace Ease
	{
		float linear(float t);
		
		float elastic_in(float t);
		float elastic_out(float t);
		float elastic_in_out(float t);

		float quadratic_in(float t);
		float quadratic_out(float t);
		float quadratic_in_out(float t);

		float sine_in(float t);
		float sine_out(float t);
		float sine_in_out(float t);

		float exp_in(float t);
		float exp_out(float t);
		float exp_in_out(float t);

		float back_in(float t);
		float back_out(float t);
		float back_in_out(float t);

		float circ_in(float t);
		float circ_out(float t);
		float circ_in_out(float t);
	}
}
