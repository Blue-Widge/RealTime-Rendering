#include "Matrix.h"
#include "Vector.h"
#include "Tools.h"

const Mat3 Mat3_Identity = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
const Mat4 Mat4_Identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

//-------------------------------------------------------------------------------------------------
// Fonctions à coder

// TODO
Mat3 Mat3_MulMM(Mat3 mat1, Mat3 mat2)
{
    Mat3 result = { 0 };
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int o = 0; o < 3; o++)
                result.data[i][j] += mat1.data[i][o] * mat2.data[o][j];
        }
    }
    return result;
}


// TODO
Vec3 Mat3_MulMV(Mat3 mat, Vec3 v)
{
    Vec3 result = { 0 };
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            mat.data[i][j] *= v.data[j];
            result.data[i] += mat.data[i][j];
        }
    }
    return result;
}

// TODO
float Mat3_Det(Mat3 mat)
{
    float detFirstPart = (mat.data[0][0] * mat.data[1][1] * mat.data[2][2])
        + (mat.data[0][1] * mat.data[1][2] * mat.data[2][0])
        + (mat.data[1][0] * mat.data[2][1] * mat.data[0][2]);

    float detSecondPart = (mat.data[2][0] * mat.data[1][1] * mat.data[0][2])
        + (mat.data[1][0] * mat.data[0][1] * mat.data[2][2])
        + (mat.data[2][1] * mat.data[1][2] * mat.data[0][0]);

    return detFirstPart - detSecondPart;
}

// TODO
Mat4 Mat4_Transpose(Mat4 matrix)
{
    Mat4 copy = matrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            matrix.data[i][j] = copy.data[j][i];
    }
    return matrix;
}

// TODO
Mat4 Mat4_MulMM(Mat4 mat1, Mat4 mat2)
{
    Mat4 result = { 0 };
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int o = 0; o < 4; o++)
                result.data[i][j] += mat1.data[i][o] * mat2.data[o][j];
        }
    }
    return result;
}
// TODO
Vec4 Mat4_MulMV(Mat4 mat, Vec4 v)
{
    Vec4 result = { 0 };
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            mat.data[i][j] *= v.data[j];
            result.data[i] += mat.data[i][j];
        }
    }
    return result;
}

// TODO
Mat4 Mat4_Scale(Mat4 mat, float s)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            mat.data[i][j] *= s;
    }
    return mat;
}

// TODO
float Mat4_Cofactor(Mat4 mat, int i, int j)
{
    int sum = i + j;
    Mat3 matrix = { 0 };
    int u = 0, v = 0;

    for (int o = 0; o < 4; ++o)
    {
        v = 0;
        if (o == i)
            continue;
        for (int p = 0; p < 4; ++p)
        {
            if (p == j)
                continue;
            matrix.data[u][v] = mat.data[o][p];
            v++;
        }
        u++;
    }

    if (sum % 2 == 0)
        return Mat3_Det(matrix);
    return Mat3_Det(matrix) * -1;
}

// TODO
Mat4 Mat4_CofactorMatrix(Mat4 mat)
{
    Mat4 result = { 0 };
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.data[i][j] = Mat4_Cofactor(mat, i, j);
        }
    }
    return result;
}

// TODO
float Mat4_Det(Mat4 mat)
{
    return mat.data[3][3] * Mat4_Cofactor(mat, 3, 3);
}

// TODO
Mat4 Mat4_Inv(Mat4 mat)
{
    float determinant = Mat4_Det(mat);
    if (!determinant)
        return Mat4_Identity;
    return Mat4_Scale(Mat4_Transpose(Mat4_CofactorMatrix(mat)), 1.f / determinant);
}

//-------------------------------------------------------------------------------------------------
// Fonctions du basecode

Mat4 Mat4_GetProjectionMatrix(float l, float r, float b, float t, float n, float f)
{
    Mat4 projectionMatrix = Mat4_Identity;

    if (r == l || t == b || f == n)
        return projectionMatrix;

    n = -n;
    f = -f;

    projectionMatrix.data[0][0] = 2.f * n / (r - l);
    projectionMatrix.data[1][1] = 2.f * n / (t - b);
    projectionMatrix.data[2][2] = (f + n) / (f - n);
    projectionMatrix.data[3][3] = 0.f;

    projectionMatrix.data[0][2] = -(r + l) / (r - l);
    projectionMatrix.data[1][2] = -(t + b) / (t - b);
    projectionMatrix.data[3][2] = 1.f;

    projectionMatrix.data[2][3] = -2.f * f * n / (f - n);

    return projectionMatrix;
}


Mat4 Mat4_GetScaleMatrix(float s)
{
    Mat4 scaleMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
        scaleMatrix.data[i][i] = s;

    return scaleMatrix;
}

Mat4 Mat4_GetScale3Matrix(Vec3 v)
{
    Mat4 scaleMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
        scaleMatrix.data[i][i] = v.data[i];

    return scaleMatrix;
}

Mat4 Mat4_GetTranslationMatrix(Vec3 v)
{
    Mat4 translationMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
    {
        translationMatrix.data[i][3] = v.data[i];
    }
    translationMatrix.data[3][3] = 1.f;

    return translationMatrix;
}

Mat4 Mat4_GetXRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[1][1] = c;
    rotationMatrix.data[2][2] = c;

    rotationMatrix.data[1][2] = -s;
    rotationMatrix.data[2][1] = s;

    return rotationMatrix;
}

Mat4 Mat4_GetYRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[0][0] = c;
    rotationMatrix.data[2][2] = c;

    rotationMatrix.data[2][0] = -s;
    rotationMatrix.data[0][2] = s;

    return rotationMatrix;
}

Mat4 Mat4_GetZRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[0][0] = c;
    rotationMatrix.data[1][1] = c;

    rotationMatrix.data[0][1] = -s;
    rotationMatrix.data[1][0] = s;

    return rotationMatrix;
}
