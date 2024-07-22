#include "Math.hpp"

#define M_RADPI	57.295779513082f

static inline float math_cr_im_atan(float x)
{
	return _mm_cvtss_f32(_mm_atan_ps(_mm_set_ss(x)));
}

static inline float math_cr_im_powf(float x, float y)
{
	return _mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(x), _mm_set_ss(y)));
}

void Math::NormalizeAngles(QAngle& angle)
{
	if (angle.x < -89.f)
		angle.x = -89.f;

	if (angle.x > 89.f)
		angle.x = 89.f;

	while (angle.y < -180.f)
		angle.y += 360.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	angle.z = 0.f;
}

QAngle Math::CalcAngle(const Vector3& src, const Vector3& dst)
{
	QAngle srca = { src.x, src.y, src.z };
	QAngle dsta = { dst.x, dst.y, dst.z };

	QAngle angle = QAngle();
	QAngle delta = srca - dsta;

	float hyp = vec_cr_im_sqrtf(math_cr_im_powf(delta.x, 2.f) + math_cr_im_powf(delta.y, 2.f));

	angle.x = (float)(math_cr_im_atan(delta.z / hyp) * M_RADPI);
	angle.y = (float)(math_cr_im_atan(delta.y / delta.x) * M_RADPI);

	if (delta.x >= 0.0)
		angle.y += 180.0f;

	angle.z = 0.f;

	return angle;
}

double Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	NormalizeAngles(delta);

	return vec_cr_im_sqrtf(math_cr_im_powf(delta.x, 2.0f) + math_cr_im_powf(delta.y, 2.0f));
}

float Math::Distance(Vector3 EntityPos, Vector3 PlayerPos)
{
	float Dist = vec_cr_im_sqrtf((PlayerPos.x - EntityPos.x) * (PlayerPos.x - EntityPos.x)
		+ (PlayerPos.y - EntityPos.y) * (PlayerPos.y - EntityPos.y)
		+ (PlayerPos.z - EntityPos.z) * (PlayerPos.z - EntityPos.z));

	return Dist * 0.0254f;
}

float Math::Distance2D(const Vector3& Src, const Vector3& Dst)
{
	return FastSqrt(math_cr_im_powf(Src.x - Dst.x, 2.f) + math_cr_im_powf(Src.y - Dst.y, 2.f) + math_cr_im_powf(Src.z - Dst.z, 2.f));
}