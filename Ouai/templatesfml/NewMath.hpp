#ifndef NEWMATH_H
#define NEWMATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Common.hpp"

/// \brief Structure for a 2x2 matrix
typedef struct Matrix2x2
{
    float m[2][2];
}Matrix2x2;

/// \brief Structure for a 3x3 matrix
typedef struct Matrix3x3
{
    float m[3][3];
}Matrix3x3;

/// \brief Structure for a 4x4 matrix
typedef struct Matrix4x4
{
    float m[4][4];
}Matrix4x4;

/// \brief Structure for a quaternion
typedef struct Quaternion
{
    float w, x, y, z;
}Quaternion;

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

/// \brief Multiplies two 4x4 matrices
/// \param a Matrix A
/// \param b Matrix B
Matrix4x4 MultiplyMatrix4x4(const Matrix4x4 _a, const Matrix4x4 _b);

#endif // NEWMATH_H