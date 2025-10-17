#ifndef NEWMATH_H
#define NEWMATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "../Common.hpp"



/// \brief Structure for a quaternion
typedef struct Quaternion
{
    float w, x, y, z;
    // Addition
    Quaternion operator+(const Quaternion& rhs) const
    {
        Quaternion result;
        result.w = w + rhs.w;
        result.x = x + rhs.x;
        result.y = y + rhs.y;
        result.z = z + rhs.z;
        return result;
    }

    // Subtraction
    Quaternion operator-(const Quaternion& rhs) const
    {
        Quaternion result;
        result.w = w - rhs.w;
        result.x = x - rhs.x;
        result.y = y - rhs.y;
        result.z = z - rhs.z;
        return result;
    }

    // Scalar multiplication
    Quaternion operator*(float scalar) const
    {
        Quaternion result;
        result.w = w * scalar;
        result.x = x * scalar;
        result.y = y * scalar;
        result.z = z * scalar;
        return result;
    }

    // Scalar division
    Quaternion operator/(float scalar) const
    {
        Quaternion result;
        result.w = w / scalar;
        result.x = x / scalar;
        result.y = y / scalar;
        result.z = z / scalar;
        return result;
    }
}Quaternion;

/// \brief Structure for a 2x2 matrix
typedef struct Matrix2x2
{
    float m[2][2];

    // Addition
    Matrix2x2 operator+(const Matrix2x2& rhs) const
    {
        Matrix2x2 result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.m[i][j] = m[i][j] + rhs.m[i][j];
        return result;
    }

    // Subtraction
    Matrix2x2 operator-(const Matrix2x2& rhs) const
    {
        Matrix2x2 result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.m[i][j] = m[i][j] - rhs.m[i][j];
        return result;
    }

    // Scalar multiplication
    Matrix2x2 operator*(float scalar) const
    {
        Matrix2x2 result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.m[i][j] = m[i][j] * scalar;
        return result;
    }

    // Scalar division
    Matrix2x2 operator/(float scalar) const
    {
        Matrix2x2 result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.m[i][j] = m[i][j] / scalar;
        return result;
    }

    // Matrix multiplication
    Matrix2x2 operator*(const Matrix2x2& rhs) const
    {
        Matrix2x2 result = {};
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    result.m[i][j] += m[i][k] * rhs.m[k][j];
        return result;
    }
} Matrix2x2;

/// \brief Structure for a 3x3 matrix
typedef struct Matrix3x3
{
    float m[3][3];

    Matrix3x3 operator+(const Matrix3x3& rhs) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] + rhs.m[i][j];
        return result;
    }

    Matrix3x3 operator-(const Matrix3x3& rhs) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] - rhs.m[i][j];
        return result;
    }

    Matrix3x3 operator*(float scalar) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] * scalar;
        return result;
    }

    Matrix3x3 operator/(float scalar) const
    {
        Matrix3x3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][j] / scalar;
        return result;
    }

    Matrix3x3 operator*(const Matrix3x3& rhs) const
    {
        Matrix3x3 result = {};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    result.m[i][j] += m[i][k] * rhs.m[k][j];
        return result;
    }
} Matrix3x3;

/// \brief Structure for a 4x4 matrix
typedef struct Matrix4x4
{
    float m[4][4];

    Matrix4x4 operator+(const Matrix4x4& rhs) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.m[i][j] = m[i][j] + rhs.m[i][j];
        return result;
    }

    Matrix4x4 operator-(const Matrix4x4& rhs) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.m[i][j] = m[i][j] - rhs.m[i][j];
        return result;
    }

    Matrix4x4 operator*(float scalar) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.m[i][j] = m[i][j] * scalar;
        return result;
    }

    Matrix4x4 operator/(float scalar) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.m[i][j] = m[i][j] / scalar;
        return result;
    }

    Matrix4x4 operator*(const Matrix4x4& rhs) const
    {
        Matrix4x4 result = {};
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    result.m[i][j] += m[i][k] * rhs.m[k][j];
        return result;
    }
	/// \brief Create a translation matrix
	/// \param t Translation vector
    static Matrix4x4 CreateTranslation(const sf::Vector3f& _t);
	/// \brief Create a scaling matrix
	/// \param s Scaling vector
    static Matrix4x4 CreateScale(const sf::Vector3f& _s);
	/// \brief Create a rotation matrix from a quaternion
	/// \param q Quaternion representing the rotation
    static Matrix4x4 CreateFromQuaternion(const Quaternion& _q);

} Matrix4x4;

typedef struct Vector4
{
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {
    }

    // Addition
    Vector4 operator+(const Vector4& rhs) const
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    // Subtraction
    Vector4 operator-(const Vector4& rhs) const
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    // Scalar multiplication
    Vector4 operator*(float scalar) const
    {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Scalar division
    Vector4 operator/(float scalar) const
    {
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }
} Vector4f;


inline Vector4 operator*(const Matrix4x4& mat, const Vector4& vec)
{
    Vector4 result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w;
    result.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w;
    result.w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w;
    return result;
}


/// \brief Transform degree angle to radian
/// \param degrees Degree angle to convert
float DegreesToRadians(float _degrees);

/// \brief Transform radian angle to degree
/// \param radians Radian angle to convert
float RadiansToDegrees(float _radians);

/// ==================== 3D VECTOR FUNCTIONS ====================

/// \brief Get the length of a 3D vector
/// \param vector Vector to evaluate
float GetLength3D(sf::Vector3f _vector);

/// \brief Get the distance between two 3D vectors
/// \param v1 First vector
/// \param v2 Second vector
float DistanceTo3D(sf::Vector3f _v1, sf::Vector3f _v2);

/// \brief Linearly interpolate between two 3D vectors
/// \param a Start vector
/// \param b End vector
/// \param t Interpolation factor
sf::Vector3f Lerp3D(sf::Vector3f _a, sf::Vector3f _b, float _t);

/// \brief Return the forward direction vector in 3D (rotation around Y axis)
/// \param angle Angle in degrees
sf::Vector3f ForwardVector3D(float _angle);

/// \brief Return the normalized direction from one 3D point to another
/// \param from Start point
/// \param to End point
sf::Vector3f LookAt3D(sf::Vector3f _from, sf::Vector3f _to);

/// \brief Normalize a 3D vector
/// \param vector Vector to normalize
sf::Vector3f Normalize3D(sf::Vector3f _vector);

/// \brief Return the dot product of two 3D vectors
/// \param v1 First vector
/// \param v2 Second vector
float DotProduct3D(sf::Vector3f _v1, sf::Vector3f _v2);

/// \brief Return the velocity vector in 3D
/// \param previous Last known position (will be updated)
/// \param current Current position
/// \param dt Delta time
sf::Vector3f GetVelocity3D(sf::Vector3f* _previous, sf::Vector3f _current, float _dt);

/// \brief Project one 3D vector onto another
/// \param vec1 Vector to project
/// \param vec2 Vector to project onto
sf::Vector3f GetProjectOnto3D(sf::Vector3f _vec1, sf::Vector3f _vec2);

/// \brief Return the cross product of two 3D vectors
/// \param a First vector
/// \param b Second vector
sf::Vector3f CrossProduct3D(sf::Vector3f _a, sf::Vector3f _b);

/// \brief Return the angle between two 3D points (in radians, XZ plane)
/// \param point1 Start point
/// \param point2 End point
float GetAngle3D(sf::Vector3f _point1, sf::Vector3f _point2);

/// \brief Estimate the distance between two 3D points (heuristic)
/// \param a First point
/// \param b Second point
float Heuristic3D(sf::Vector3f _a, sf::Vector3f _b);

/// ==================== 2D VECTOR FUNCTIONS ====================

/// \brief Get the length of a 2D vector
/// \param vector Vector to evaluate
float GetLength2D(sf::Vector2f _vector);

/// \brief Get the distance between two 2D vectors
/// \param v1 First vector
/// \param v2 Second vector
float DistanceTo2D(sf::Vector2f _v1, sf::Vector2f _v2);

/// \brief Linearly interpolate between two 2D vectors
/// \param a Start vector
/// \param b End vector
/// \param t Interpolation factor
sf::Vector2f Lerp2D(sf::Vector2f _a, sf::Vector2f _b, float _t);

/// \brief Return the forward direction vector in 2D (angle in degrees)
/// \param angle Angle in degrees
sf::Vector2f ForwardVector2D(float _angle);

/// \brief Return the normalized direction from one 2D point to another
/// \param from Start point
/// \param to End point
sf::Vector2f LookAt2D(sf::Vector2f _from, sf::Vector2f _to);

/// \brief Normalize a 2D vector
/// \param vector Vector to normalize
sf::Vector2f Normalize2D(sf::Vector2f _vector);

/// \brief Return the dot product of two 2D vectors
/// \param v1 First vector
/// \param v2 Second vector
float DotProduct2D(sf::Vector2f _v1, sf::Vector2f _v2);

/// \brief Return the velocity vector in 2D
/// \param previous Last known position (will be updated)
/// \param current Current position
/// \param dt Delta time
sf::Vector2f GetVelocity2D(sf::Vector2f* _previous, sf::Vector2f _current, float _dt);

/// \brief Project one 2D vector onto another
/// \param vec1 Vector to project
/// \param vec2 Vector to project onto
sf::Vector2f GetProjectOnto2D(sf::Vector2f _vec1, sf::Vector2f _vec2);

/// \brief Return the cross product (z component) of two 2D vectors
/// \param a First vector
/// \param b Second vector
float CrossProduct2D(sf::Vector2f _a, sf::Vector2f _b);

/// \brief Return the angle between two 2D points (in radians)
/// \param point1 Start point
/// \param point2 End point
float GetAngle2D(sf::Vector2f _point1, sf::Vector2f _point2);

/// \brief Estimate the distance between two 2D points (heuristic)
/// \param a First point
/// \param b Second point
float Heuristic2D(sf::Vector2f _a, sf::Vector2f _b);

/// ==================== AUTRES OUTILS ====================

/// \brief Clamp a value between min and max
/// \param current Value to clamp
/// \param min Minimum value
/// \param max Maximum value
float Clamp(float _current, float _min, float _max);

/// \brief Always return positive value
/// \param value Value to get positive
float ABS(float _value);

/// \brief Returns the product of two quaternions
/// \param a First quaternion
/// \param b Second quaternion
Quaternion QuaternionProduct(Quaternion _a, Quaternion _b);

/// \brief Applies a quaternion rotation to a 3D vector
/// \param v Input vector
/// \param q Quaternion rotation
sf::Vector3f ApplyQuaternionRotation(sf::Vector3f _v, Quaternion _q);

/// \brief Applies a quaternion normalization like for vector
/// \param q Quaternion
Quaternion NormalizeQuaternion(Quaternion _q);
/// \brief Creates a rotation matrix around the Y axis
/// \param angle Rotation angle in radians
Matrix4x4& MakeRotationY(float _angle);
/// \brief Applies a 4x4 matrix transformation to a 3D vector
/// \param m Transformation matrix
/// \param v Input vector
sf::Vector3f ApplyMatrix4D(const Matrix4x4& _m, const sf::Vector3f& _v);
/// \brief Applies a 3x3 matrix transformation to a 3D vector
/// \param m Transformation matrix
/// \param v Input vector
sf::Vector3f ApplyMatrix3D(const Matrix3x3& _m, const sf::Vector3f& _v);
/// \brief Applies a 2x2 matrix transformation to a 2D vector
/// \param m Transformation matrix
/// \param v Input vector
sf::Vector2f ApplyMatrix2D(const Matrix2x2& _m, const sf::Vector2f& _v);
/// \brief Creates a quaternion from an angle around the Y axis
/// \param angle Rotation angle in radians
Quaternion QuaternionFromAngleY(float _angle);
/// \brief Creates a quaternion from Euler angles (in degrees)
/// \param euler Euler angles (pitch, yaw, roll)
Quaternion QuaternionFromEuler(const sf::Vector3f& _euler);
/// \brief Converts a 3x3 rotation matrix to a quaternion
/// \param m Rotation matrix
Quaternion Matrix3x3ToQuaternion(const Matrix3x3& _m);
/// \brief Converts a quaternion to Euler angles (in degrees)
/// \param q Input quaternion
sf::Vector3f EulerFormQuaternion(const Quaternion& _q);
/// \brief Linearly interpolates between two quaternions
/// \param a Start quaternion
/// \param b End quaternion
/// \param t Interpolation factor
Quaternion LerpQuaternion(const Quaternion& _a, const Quaternion& _b, float _t);
/// \brief Computes the inverse of a 4x4 matrix
/// \param mat Input matrix
Matrix4x4 InverseMatrix4x4(const Matrix4x4& _mat);
/// \brief Computes the inverse of a quaternion
/// \param q Input quaternion
Quaternion InverseQuaternion(const Quaternion& _q);

#endif // NEWMATH_H