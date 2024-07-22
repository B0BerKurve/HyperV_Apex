#include "Vector4D.hpp"

#include <limits>

void VectorCopy(const Vector4D& src, Vector4D& dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}
void VectorLerp(const Vector4D& src1, const Vector4D& src2, float t, Vector4D& dest)
{
    dest.x = src1.x + (src2.x - src1.x) * t;
    dest.y = src1.y + (src2.y - src1.y) * t;
    dest.z = src1.z + (src2.z - src1.z) * t;
    dest.w = src1.w + (src2.w - src1.w) * t;
}
float VectorLength(const Vector4D& v)
{
    return vec_cr_im_sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

float NormalizeVector(Vector4D& v)
{
    float l = v.Length();
    if (l != 0.0f) {
        v /= l;
    }
    else {
        v.x = v.y = v.z = v.w = 0.0f;
    }
    return l;
}

Vector4D::Vector4D(void)
{
    Invalidate();
}
Vector4D::Vector4D(float X, float Y, float Z, float W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}
Vector4D::Vector4D(float* clr)
{
    x = clr[0];
    y = clr[1];
    z = clr[2];
    w = clr[3];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void Vector4D::Init(float ix, float iy, float iz, float iw)
{
    x = ix; y = iy; z = iz; w = iw;
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
void Vector4D::Zero()
{
    x = y = z = w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

Vector4D& Vector4D::operator=(const Vector4D& vOther)
{
    x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
    return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
float& Vector4D::operator[](int i)
{
    return ((float*)this)[i];
}

float Vector4D::operator[](int i) const
{
    return ((float*)this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
float* Vector4D::Base()
{
    return (float*)this;
}

float const* Vector4D::Base() const
{
    return (float const*)this;
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

void Vector4D::Invalidate()
{
    //#ifdef _DEBUG
    //#ifdef VECTOR_PARANOIA
    x = y = z = w = 99999.f;
    //#endif
    //#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool Vector4D::operator==(const Vector4D& src) const
{
    return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

bool Vector4D::operator!=(const Vector4D& src) const
{
    return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void Vector4D::CopyToArray(float* rgfl) const
{
    rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------
// #pragma message("TODO: these should be SSE")

void Vector4D::Negate()
{
    x = -x; y = -y; z = -z; w = -w;
}

// Get the component of this vector parallel to some other given vector
Vector4D Vector4D::ProjectOnto(const Vector4D& onto)
{
    return onto * (this->Dot(onto) / (onto.LengthSqr()));
}

// FIXME: Remove
// For backwards compatability
void Vector4D::MulAdd(const Vector4D& a, const Vector4D& b, float scalar)
{
    x = a.x + b.x * scalar;
    y = a.y + b.y * scalar;
    z = a.z + b.z * scalar;
    w = a.w + b.w * scalar;
}

Vector4D VectorLerp(const Vector4D& src1, const Vector4D& src2, float t)
{
    Vector4D result;
    VectorLerp(src1, src2, t, result);
    return result;
}

float Vector4D::Dot(const Vector4D& b) const
{
    return (x * b.x + y * b.y + z * b.z + w * b.w);
}
void VectorClear(Vector4D& a)
{
    a.x = a.y = a.z = a.w = 0.0f;
}

float Vector4D::Length(void) const
{
    return vec_cr_im_sqrtf(x * x + y * y + z * z + w * w);
}

//-----------------------------------------------------------------------------
// Get the distance from this vector to the other one 
//-----------------------------------------------------------------------------
float Vector4D::DistTo(const Vector4D& vOther) const
{
    Vector4D delta;
    delta = *this - vOther;
    return delta.Length();
}

//-----------------------------------------------------------------------------
// Returns a vector with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------
Vector4D Vector4D::Min(const Vector4D& vOther) const
{
    return Vector4D(x < vOther.x ? x : vOther.x,
        y < vOther.y ? y : vOther.y,
        z < vOther.z ? z : vOther.z,
        w < vOther.w ? w : vOther.w);
}

Vector4D Vector4D::Max(const Vector4D& vOther) const
{
    return Vector4D(x > vOther.x ? x : vOther.x,
        y > vOther.y ? y : vOther.y,
        z > vOther.z ? z : vOther.z,
        w > vOther.w ? w : vOther.w);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

Vector4D Vector4D::operator-(void) const
{
    return Vector4D(-x, -y, -z, -w);
}

Vector4D Vector4D::operator+(const Vector4D& v) const
{
    return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4D Vector4D::operator-(const Vector4D& v) const
{
    return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4D Vector4D::operator*(float fl) const
{
    return Vector4D(x * fl, y * fl, z * fl, w * fl);
}

Vector4D Vector4D::operator*(const Vector4D& v) const
{
    return Vector4D(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4D Vector4D::operator/(float fl) const
{
    return Vector4D(x / fl, y / fl, z / fl, w / fl);
}

Vector4D Vector4D::operator/(const Vector4D& v) const
{
    return Vector4D(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4D operator*(float fl, const Vector4D& v)
{
    return v * fl;
}