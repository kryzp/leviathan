#include <lev/math/quaternion.h>

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

const Quaternion& Quaternion::zero()	{ static const Quaternion ZERO	= Quaternion(0.0f); return ZERO;	}
const Quaternion& Quaternion::one()	{ static const Quaternion ONE	= Quaternion(1.0f); return ONE;		}
