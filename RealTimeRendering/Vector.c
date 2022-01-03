#include "Vector.h"

const Vec2 Vec2_Right = { +1.0f, +0.0f };
const Vec2 Vec2_Left  = { -1.0f, +0.0f };
const Vec2 Vec2_Up    = { +0.0f, +1.0f };
const Vec2 Vec2_Down  = { +0.0f, -1.0f };
const Vec2 Vec2_Zero  = { +0.0f, +0.0f };
const Vec2 Vec2_One   = { +1.0f, +1.0f };

const Vec3 Vec3_Right = { +1.0f, +0.0f, +0.0f };
const Vec3 Vec3_Left  = { -1.0f, +0.0f, +0.0f };
const Vec3 Vec3_Up    = { +0.0f, +1.0f, +0.0f };
const Vec3 Vec3_Down  = { +0.0f, -1.0f, +0.0f };
const Vec3 Vec3_Front = { +0.0f, +0.0f, +1.0f };
const Vec3 Vec3_Back  = { +0.0f, +0.0f, -1.0f };
const Vec3 Vec3_Zero  = { +0.0f, +0.0f, +0.0f };
const Vec3 Vec3_One   = { +1.0f, +1.0f, +1.0f };

const Vec4 Vec4_RightH = { +1.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_LeftH  = { -1.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_UpH    = { +0.0f, +1.0f, +0.0f, +1.0f };
const Vec4 Vec4_DownH  = { +0.0f, -1.0f, +0.0f, +1.0f };
const Vec4 Vec4_FrontH = { +0.0f, +0.0f, +1.0f, +1.0f };
const Vec4 Vec4_BackH  = { +0.0f, +0.0f, -1.0f, +1.0f };
const Vec4 Vec4_ZeroH  = { +0.0f, +0.0f, +0.0f, +1.0f };
const Vec4 Vec4_Zero   = { +0.0f, +0.0f, +0.0f, +0.0f };
const Vec4 Vec4_One    = { +1.0f, +1.0f, +1.0f, +1.0f };

//-------------------------------------------------------------------------------------------------
// Fonctions à coder

// TODO
Vec2 Vec2_Add(Vec2 v1, Vec2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}


// TODO
Vec2 Vec2_Sub(Vec2 v1, Vec2 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}


// TODO
float Vec2_SignedArea(const Vec2 a, const Vec2 b, const Vec2 c)
{
    Vec2 x = Vec2_Sub(b, a);
    Vec2 y = Vec2_Sub(c, a);
    return (x.x * y.y) - (x.y * y.x);
}

// TODO
bool Vec2_Barycentric(Vec2* vertices, Vec2 p, float* bary)
{
    Vec2 a = vertices[0];
    Vec2 b = vertices[1];
    Vec2 c = vertices[2];

    Vec2 bmp = Vec2_Sub(b, p);
    Vec2 cmp = Vec2_Sub(c, p);
    Vec2 bma = Vec2_Sub(b, a);
    Vec2 cma = Vec2_Sub(c, a);
    Vec2 amp = Vec2_Sub(a, p);

    float bas = (bma.x * cma.y) - (bma.y * cma.x);

    bary[0] = ((bmp.x * cmp.y) - (bmp.y * cmp.x)) / bas;
    bary[1] = ((cmp.x * amp.y) - (cmp.y * amp.x)) / bas;
    bary[2] = ((amp.x * bmp.y) - (amp.y * bmp.x)) / bas;
    return (bary[0] > 0 && bary[1] > 0 && bary[2] > 0) ? true : false;
}

// TODO
Vec3 Vec3_Add(Vec3 v1, Vec3 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

// TODO
Vec3 Vec3_Sub(Vec3 v1, Vec3 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

// TODO
Vec3 Vec3_Scale(Vec3 v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

// TODO
float Vec3_Dot(Vec3 v1, Vec3 v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// TODO
Vec3 Vec3_Cross(Vec3 v1, Vec3 v2)
{
    Vec3 cross = { 0 };
    cross.x = (v1.y * v2.z) - (v1.z * v2.y);
    cross.y = (v1.z * v2.x) - (v1.x * v2.z);
    cross.z = (v1.x * v2.y) - (v1.y * v2.x);
    return cross;
}

// TODO
float Vec3_Length(Vec3 v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

// TODO
Vec3 Vec3_Normalize(Vec3 v)
{
    float norme = Vec3_Length(v);
    if (norme == 1)
        return v;
    return Vec3_Scale(v, 1.f / norme);
}

// TODO
Vec3 Vec3_From4(Vec4 v)
{
    Vec3 current = { 0 };
    v.w = v.w == 0 ? 1 : v.w;
    current.x = v.x / v.w;
    current.y = v.y / v.w;
    current.z = v.z / v.w;
    return current;
}

//-------------------------------------------------------------------------------------------------
// Fonctions du basecode

Vec3 Vec3_Mul(Vec3 v1, Vec3 v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    return v1;
}

Vec3 Vec3_Max(Vec3 v1, Vec3 v2)
{
    v1.x = fmaxf(v1.x, v2.x);
    v1.y = fmaxf(v1.y, v2.y);
    v1.z = fmaxf(v1.z, v2.z);
    return v1;
}

Vec3 Vec3_Min(Vec3 v1, Vec3 v2)
{
    v1.x = fminf(v1.x, v2.x);
    v1.y = fminf(v1.y, v2.y);
    v1.z = fminf(v1.z, v2.z);
    return v1;
}
