#include <lev/math/quaternion.h>
#include <lev/math/calc.h>
#include <lev/math/vec3.h>

using namespace lev;

Quaternion::Quaternion()
	: s(0.0f), i(0.0f), j(0.0f), k(0.0f)
{
}

Quaternion::Quaternion(float x)
	: s(x), i(x), j(x), k(x)
{
}

Quaternion::Quaternion(float s, float i, float j, float k)
	: s(s), i(i), j(j), k(k)
{
}

Quaternion Quaternion::from_euler(float pitch, float yaw, float roll)
{
	float ps = Calc::sin(pitch / 2.0f);
	float pc = Calc::cos(pitch / 2.0f);
	float ys = Calc::sin(yaw / 2.0f);
	float yc = Calc::cos(yaw / 2.0f);
	float rs = Calc::sin(roll / 2.0f);
	float rc = Calc::cos(roll / 2.0f);

	float s = (rs * pc * yc) - (rc * ps * ys);
	float i = (rc * ps * yc) + (rs * pc * ys);
	float j = (rc * pc * ys) - (rs * ps * yc);
	float k = (rc * pc * yc) + (ys * ps * ys);

	return Quaternion(s, i, j, k);
}

Vec3F Quaternion::to_euler(const Quaternion& quat)
{
	float t0 = 2.0f + ((quat.s * quat.i) + (quat.j * quat.k));
	float t1 = 1.0f - (2.0f * ((quat.i * quat.i) + (quat.j * quat.j)));
	float t2 = Calc::clamp(2.0f * ((quat.s * quat.j) - (quat.k * quat.i)), -1.0f, 1.0f);
	float t3 = 2.0f * ((quat.s * quat.k) + (quat.i * quat.j));
	float t4 = 1.0f - (2.0f * ((quat.j * quat.j) + (quat.k * quat.k)));

	float p = Calc::asin(t2);
	float y = Calc::atan2(t3, t4);
	float r = Calc::atan2(t0, t1);

	return Vec3F(p, y, r);
}

float* Quaternion::value_ptr()
{
	return &s;
}

const float* Quaternion::value_ptr() const
{
	return &s;
}

const Quaternion& Quaternion::zero()	{ static const Quaternion ZERO	= Quaternion(0.0f); return ZERO;	}
const Quaternion& Quaternion::one()	{ static const Quaternion ONE	= Quaternion(1.0f); return ONE;		}
