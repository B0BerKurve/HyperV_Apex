#include "Vector.hpp"

namespace Math
{
	void NormalizeAngles(QAngle& angle);
	double GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
	QAngle CalcAngle(const Vector3& src, const Vector3& dst);

	float Distance(Vector3 EntityPos, Vector3 PlayerPos);
	float Distance2D(const Vector3& Src, const Vector3& Dst);
}