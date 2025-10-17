///////////////////////////////////////////////////////////////////////
///////////////////////Bibliothèque de NewMath/////////////////////////
///////////////////////////////////////////////////////////////////////

#include "NewMath.hpp"

// ==================== Fonctions 3D existantes ====================

float DegreesToRadians(float _degrees)
{
    return _degrees * ((float)M_PI / 180);
}

float RadiansToDegrees(float _radians)
{
    return _radians * (180 / (float)M_PI);
}

float GetLength3D(sf::Vector3f _vector)
{
    // Retourne la longueur d'un vecteur 3D
    return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z);
}

float DistanceTo3D(sf::Vector3f _v1, sf::Vector3f _v2)
{
    // Retourne la distance entre deux vecteurs 3D
    sf::Vector3f vector;
    vector.x = _v1.x - _v2.x;
    vector.y = _v1.y - _v2.y;
    vector.z = _v1.z - _v2.z;
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

sf::Vector3f Lerp3D(sf::Vector3f _a, sf::Vector3f _b, float _t)
{
    // Interpolation linéaire entre deux vecteurs 3D
    sf::Vector3f vector;
    vector.x = _a.x * (1 - _t) + _b.x * _t;
    vector.y = _a.y * (1 - _t) + _b.y * _t;
    vector.z = _a.z * (1 - _t) + _b.z * _t;
    return vector;
}

sf::Vector3f ForwardVector3D(float _angle)
{
    // Retourne le vecteur forward en 3D (rotation autour de Y)
    sf::Vector3f forwardVector;
    forwardVector.x = cosf(DegreesToRadians(_angle));
    forwardVector.y = 0.f;
    forwardVector.z = sinf(DegreesToRadians(_angle));
    return Normalize3D(forwardVector);
}

sf::Vector3f LookAt3D(sf::Vector3f _from, sf::Vector3f _to)
{
    // Retourne la direction normalisée entre deux points 3D
    sf::Vector3f direction;
    direction.x = _to.x - _from.x;
    direction.y = _to.y - _from.y;
    direction.z = _to.z - _from.z;
    return Normalize3D(direction);
}

sf::Vector3f Normalize3D(sf::Vector3f _vector)
{
    // Normalise un vecteur 3D
    float length = GetLength3D(_vector);
    if (length != 0) {
        _vector.x /= length;
        _vector.y /= length;
        _vector.z /= length;
    }
    return _vector;
}

float DotProduct3D(sf::Vector3f _v1, sf::Vector3f _v2)
{
    // Produit scalaire de deux vecteurs 3D
    return (_v1.x * _v2.x) + (_v1.y * _v2.y) + (_v1.z * _v2.z);
}

sf::Vector3f GetVelocity3D(sf::Vector3f* _previous, sf::Vector3f _current, float _dt)
{
    // Retourne la vélocité en 3D
    sf::Vector3f velocity;
    velocity.x = (_current.x - _previous->x) * _dt;
    velocity.y = (_current.y - _previous->y) * _dt;
    velocity.z = (_current.z - _previous->z) * _dt;
    *_previous = _current;
    return velocity;
}

sf::Vector3f GetProjectOnto3D(sf::Vector3f _vec1, sf::Vector3f _vec2)
{
    // Projection d'un vecteur 3D sur un autre
    float dotProduct = DotProduct3D(_vec1, _vec2);
    float magnitude = GetLength3D(_vec2);
    if (magnitude == 0)
        return sf::Vector3f(0, 0, 0);
    float scale = dotProduct / (magnitude * magnitude);
    return sf::Vector3f(_vec2.x * scale, _vec2.y * scale, _vec2.z * scale);
}

sf::Vector3f CrossProduct3D(sf::Vector3f _a, sf::Vector3f _b)
{
    // Produit vectoriel 3D
    return sf::Vector3f(
        _a.y * _b.z - _a.z * _b.y,
        _a.z * _b.x - _a.x * _b.z,
        _a.x * _b.y - _a.y * _b.x
    );
}

float GetAngle3D(sf::Vector3f _point1, sf::Vector3f _point2)
{
    // Angle entre deux points 3D dans le plan XZ
    sf::Vector3f delta(_point2.x - _point1.x, _point2.y - _point1.y, _point2.z - _point1.z);
    return atan2f(delta.z, delta.x);
}

float Heuristic3D(sf::Vector3f _a, sf::Vector3f _b)
{
    // Heuristique de distance 3D
    return DistanceTo3D(_a, _b);
}

// ==================== Fonctions 2D ajoutées ====================

float GetLength2D(sf::Vector2f _vector)
{
    // Retourne la longueur d'un vecteur 2D
    return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
}

float DistanceTo2D(sf::Vector2f _v1, sf::Vector2f _v2)
{
    // Retourne la distance entre deux vecteurs 2D
    sf::Vector2f vector;
    vector.x = _v1.x - _v2.x;
    vector.y = _v1.y - _v2.y;
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f Lerp2D(sf::Vector2f _a, sf::Vector2f _b, float _t)
{
    // Interpolation linéaire entre deux vecteurs 2D
    sf::Vector2f vector;
    vector.x = _a.x * (1 - _t) + _b.x * _t;
    vector.y = _a.y * (1 - _t) + _b.y * _t;
    return vector;
}

sf::Vector2f ForwardVector2D(float _angle)
{
    // Retourne le vecteur forward en 2D (angle en degrés)
    sf::Vector2f forwardVector;
    forwardVector.x = cosf(DegreesToRadians(_angle));
    forwardVector.y = sinf(DegreesToRadians(_angle));
    return Normalize2D(forwardVector);
}

sf::Vector2f LookAt2D(sf::Vector2f _from, sf::Vector2f _to)
{
    // Retourne la direction normalisée entre deux points 2D
    sf::Vector2f direction;
    direction.x = _to.x - _from.x;
    direction.y = _to.y - _from.y;
    return Normalize2D(direction);
}

sf::Vector2f Normalize2D(sf::Vector2f _vector)
{
    // Normalise un vecteur 2D
    float length = GetLength2D(_vector);
    if (length != 0)
    {
        _vector.x /= length;
        _vector.y /= length;
    }
    return _vector;
}

float DotProduct2D(sf::Vector2f _v1, sf::Vector2f _v2)
{
    // Produit scalaire de deux vecteurs 2D
    return (_v1.x * _v2.x) + (_v1.y * _v2.y);
}

sf::Vector2f GetVelocity2D(sf::Vector2f* _previous, sf::Vector2f _current, float _dt)
{
    // Retourne la vélocité en 2D
    sf::Vector2f velocity;
    velocity.x = (_current.x - _previous->x) * _dt;
    velocity.y = (_current.y - _previous->y) * _dt;
    *_previous = _current;
    return velocity;
}

sf::Vector2f GetProjectOnto2D(sf::Vector2f _vec1, sf::Vector2f _vec2)
{
    // Projection d'un vecteur 2D sur un autre
    float dotProduct = DotProduct2D(_vec1, _vec2);
    float magnitude = GetLength2D(_vec2);
    if (magnitude == 0)
        return sf::Vector2f(0, 0);
    float scale = dotProduct / (magnitude * magnitude);
    return sf::Vector2f(_vec2.x * scale, _vec2.y * scale);
}

float CrossProduct2D(sf::Vector2f _a, sf::Vector2f _b)
{
    // Produit vectoriel 2D (retourne le z)
    return _a.x * _b.y - _a.y * _b.x;
}

float GetAngle2D(sf::Vector2f _point1, sf::Vector2f _point2)
{
    // Angle entre deux points 2D (en radians)
    sf::Vector2f delta(_point2.x - _point1.x, _point2.y - _point1.y);
    return atan2f(delta.y, delta.x);
}

float Heuristic2D(sf::Vector2f _a, sf::Vector2f _b)
{
    // Heuristique de distance 2D
    return DistanceTo2D(_a, _b);
}

// ==================== Fonctions utilitaires et quaternion ====================

float Clamp(float _current, float _min, float _max)
{
    if (_current < _min)
    {
        return _min;
    }
    else if (_current > _max)
    {
        return _max;
    }
    else
    {
        return _current;
    }
}

float ABS(float _value)
{
    return (_value < 0) ? -_value : _value;
}

Quaternion QuaternionProduct(Quaternion _a, Quaternion _b)
{
    return {
        _a.w * _b.w - _a.x * _b.x - _a.y * _b.y - _a.z * _b.z,
        _a.w * _b.x + _a.x * _b.w + _a.y * _b.z - _a.z * _b.y,
        _a.w * _b.y - _a.x * _b.z + _a.y * _b.w + _a.z * _b.x,
        _a.w * _b.z + _a.x * _b.y - _a.y * _b.x + _a.z * _b.w
    };
}

sf::Vector3f ApplyQuaternionRotation(sf::Vector3f _v, Quaternion _q)
{
    Quaternion vq{ 0, _v.x, _v.y, _v.z };
    Quaternion qc{ _q.w, -_q.x, -_q.y, -_q.z };
    Quaternion r = QuaternionProduct(QuaternionProduct(_q, vq), qc);
    return sf::Vector3f(r.x, r.y, r.z);
}

Quaternion NormalizeQuaternion(Quaternion _q)
{
    float _len = std::sqrt(_q.w * _q.w + _q.x * _q.x + _q.y * _q.y + _q.z * _q.z);
    if (_len == 0) return Quaternion{ 1,0,0,0 };
    return { _q.w / _len, _q.x / _len, _q.y / _len, _q.z / _len };
}

Matrix4x4& MakeRotationY(float _angle)
{
    // angle en radians
    Matrix4x4 m = {};
    m.m[0][0] = cosf(_angle);  m.m[0][1] = 0; m.m[0][2] = sinf(_angle); m.m[0][3] = 0;
    m.m[1][0] = 0;            m.m[1][1] = 1; m.m[1][2] = 0;           m.m[1][3] = 0;
    m.m[2][0] = -sinf(_angle); m.m[2][1] = 0; m.m[2][2] = cosf(_angle); m.m[2][3] = 0;
    m.m[3][0] = 0;            m.m[3][1] = 0; m.m[3][2] = 0;           m.m[3][3] = 1;
    return m;
}

sf::Vector3f ApplyMatrix3D(const Matrix3x3& _m, const sf::Vector3f& _v)
{
    // Multiplie un vecteur 3D par une matrice 3x3 (en ignorant la translation)
    return sf::Vector3f(
        _m.m[0][0] * _v.x + _m.m[0][1] * _v.y + _m.m[0][2] * _v.z,
        _m.m[1][0] * _v.x + _m.m[1][1] * _v.y + _m.m[1][2] * _v.z,
        _m.m[2][0] * _v.x + _m.m[2][1] * _v.y + _m.m[2][2] * _v.z
    );
}

sf::Vector3f ApplyMatrix4D(const Matrix4x4& _m, const sf::Vector3f& _v)
{
    // Multiplie un vecteur 3D par une matrice 4x4 (en incluant la translation)
    return sf::Vector3f(
        _m.m[0][0] * _v.x + _m.m[0][1] * _v.y + _m.m[0][2] * _v.z + _m.m[0][3],
        _m.m[1][0] * _v.x + _m.m[1][1] * _v.y + _m.m[1][2] * _v.z + _m.m[1][3],
        _m.m[2][0] * _v.x + _m.m[2][1] * _v.y + _m.m[2][2] * _v.z + _m.m[2][3]
	);
}

sf::Vector2f ApplyMatrix2D(const Matrix2x2& _m, const sf::Vector2f& _v)
{
    // Multiplie un vecteur 2D par une matrice 2x2 (en ignorant la translation)
    return sf::Vector2f(
        _m.m[0][0] * _v.x + _m.m[0][1] * _v.y,
        _m.m[1][0] * _v.x + _m.m[1][1] * _v.y
	);
}

Quaternion QuaternionFromAngleY(float _angle)
{
    // angle en radians
    float half = _angle * 0.5f;
    Quaternion q;
    q.w = cosf(half);
    q.x = 0.f;
    q.y = sinf(half);
    q.z = 0.f;
    return q;
}

Quaternion QuaternionFromEuler(const sf::Vector3f& _euler)
{
    // Convertit des angles d'Euler (en degrés) en quaternion
    float cy = cosf(DegreesToRadians(_euler.z) * 0.5f);
    float sy = sinf(DegreesToRadians(_euler.z) * 0.5f);
    float cp = cosf(DegreesToRadians(_euler.y) * 0.5f);
    float sp = sinf(DegreesToRadians(_euler.y) * 0.5f);
    float cr = cosf(DegreesToRadians(_euler.x) * 0.5f);
    float sr = sinf(DegreesToRadians(_euler.x) * 0.5f);
    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    return NormalizeQuaternion(q);
}

Quaternion Matrix3x3ToQuaternion(const Matrix3x3& _m)
{
    Quaternion q;
    float trace = _m.m[0][0] + _m.m[1][1] + _m.m[2][2];
    if (trace > 0) {
        float s = 0.5f / sqrtf(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (_m.m[2][1] - _m.m[1][2]) * s;
        q.y = (_m.m[0][2] - _m.m[2][0]) * s;
        q.z = (_m.m[1][0] - _m.m[0][1]) * s;
    }
    else {
        if (_m.m[0][0] > _m.m[1][1] && _m.m[0][0] > _m.m[2][2]) {
            float s = 2.0f * sqrtf(1.0f + _m.m[0][0] - _m.m[1][1] - _m.m[2][2]);
            q.w = (_m.m[2][1] - _m.m[1][2]) / s;
            q.x = 0.25f * s;
            q.y = (_m.m[0][1] + _m.m[1][0]) / s;
            q.z = (_m.m[0][2] + _m.m[2][0]) / s;
        }
        else if (_m.m[1][1] > _m.m[2][2]) {
            float s = 2.0f * sqrtf(1.0f + _m.m[1][1] - _m.m[0][0] - _m.m[2][2]);
            q.w = (_m.m[0][2] - _m.m[2][0]) / s;
            q.x = (_m.m[0][1] + _m.m[1][0]) / s;
            q.y = 0.25f * s;
            q.z = (_m.m[1][2] + _m.m[2][1]) / s;
        }
        else {
			float s = 2.0f * sqrtf(1.0f + _m.m[2][2]
                - _m.m[0][0] - _m.m[1][1]);
            q.w = (_m.m[1][0] - _m.m[0][1]) / s;
            q.x = (_m.m[0][2] + _m.m[2][0]) / s;
            q.y = (_m.m[1][2] + _m.m[2][1]) / s;
            q.z = 0.25f * s;
        }
    }
	return NormalizeQuaternion(q);
}

sf::Vector3f EulerFormQuaternion(const Quaternion& _q)
{
    sf::Vector3f euler;
    // Roll (X-axis rotation)
    float sinr_cosp = 2 * (_q.w * _q.x + _q.y * _q.z);
    float cosr_cosp = 1 - 2 * (_q.x * _q.x + _q.y * _q.y);
    euler.x = RadiansToDegrees(atan2f(sinr_cosp, cosr_cosp));
    // Pitch (Y-axis rotation)
    float sinp = 2 * (_q.w * _q.y - _q.z * _q.x);
    if (ABS(sinp) >= 1)
        euler.y = RadiansToDegrees(copysignf(M_PI / 2, sinp)); // use 90 degrees if out of range
    else
        euler.y = RadiansToDegrees(asinf(sinp));
    // Yaw (Z-axis rotation)
    float siny_cosp = 2 * (_q.w * _q.z + _q.x * _q.y);
    float cosy_cosp = 1 - 2 * (_q.y * _q.y + _q.z * _q.z);
    euler.z = RadiansToDegrees(atan2f(siny_cosp, cosy_cosp));
    return euler;
}

Quaternion LerpQuaternion(const Quaternion& _a, const Quaternion& _b, float _t)
{
    Quaternion result;
    result.w = _a.w + (_b.w - _a.w) * _t;
    result.x = _a.x + (_b.x - _a.x) * _t;
    result.y = _a.y + (_b.y - _a.y) * _t;
    result.z = _a.z + (_b.z - _a.z) * _t;
    return NormalizeQuaternion(result);
}

Matrix4x4 Matrix4x4::CreateTranslation(const sf::Vector3f& _t)
{
    Matrix4x4 mat = {};
    mat.m[0][0] = 1.f; mat.m[0][1] = 0.f; mat.m[0][2] = 0.f; mat.m[0][3] = _t.x;
    mat.m[1][0] = 0.f; mat.m[1][1] = 1.f; mat.m[1][2] = 0.f; mat.m[1][3] = _t.y;
    mat.m[2][0] = 0.f; mat.m[2][1] = 0.f; mat.m[2][2] = 1.f; mat.m[2][3] = _t.z;
    mat.m[3][0] = 0.f; mat.m[3][1] = 0.f; mat.m[3][2] = 0.f; mat.m[3][3] = 1.f;
    return mat;
}

Matrix4x4 Matrix4x4::CreateScale(const sf::Vector3f& _s)
{
    Matrix4x4 mat = {};
    mat.m[0][0] = _s.x; mat.m[0][1] = 0.f; mat.m[0][2] = 0.f; mat.m[0][3] = 0.f;
    mat.m[1][0] = 0.f; mat.m[1][1] = _s.y; mat.m[1][2] = 0.f; mat.m[1][3] = 0.f;
    mat.m[2][0] = 0.f; mat.m[2][1] = 0.f; mat.m[2][2] = _s.z; mat.m[2][3] = 0.f;
    mat.m[3][0] = 0.f; mat.m[3][1] = 0.f; mat.m[3][2] = 0.f; mat.m[3][3] = 1.f;
    return mat;
}

Matrix4x4 Matrix4x4::CreateFromQuaternion(const Quaternion& _q)
{
    Matrix4x4 mat = {};
    float xx = _q.x * _q.x;
    float yy = _q.y * _q.y;
    float zz = _q.z * _q.z;
    float xy = _q.x * _q.y;
    float xz = _q.x * _q.z;
    float yz = _q.y * _q.z;
    float wx = _q.w * _q.x;
    float wy = _q.w * _q.y;
    float wz = _q.w * _q.z;

    mat.m[0][0] = 1.f - 2.f * (yy + zz);
    mat.m[0][1] = 2.f * (xy - wz);
    mat.m[0][2] = 2.f * (xz + wy);
    mat.m[0][3] = 0.f;

    mat.m[1][0] = 2.f * (xy + wz);
    mat.m[1][1] = 1.f - 2.f * (xx + zz);
    mat.m[1][2] = 2.f * (yz - wx);
    mat.m[1][3] = 0.f;

    mat.m[2][0] = 2.f * (xz - wy);
    mat.m[2][1] = 2.f * (yz + wx);
    mat.m[2][2] = 1.f - 2.f * (xx + yy);
    mat.m[2][3] = 0.f;

    mat.m[3][0] = 0.f;
    mat.m[3][1] = 0.f;
    mat.m[3][2] = 0.f;
    mat.m[3][3] = 1.f;

    return mat;
}

// Inverse d'une matrice 4x4 (méthode de Gauss-Jordan, adaptée pour les matrices de transformation)
Matrix4x4 InverseMatrix4x4(const Matrix4x4& _mat)
{
    Matrix4x4 inv = {};
    float m[16], invOut[16];
    // Copie la matrice dans un tableau 1D
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i * 4 + j] = _mat.m[i][j];

    invOut[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    invOut[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    invOut[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    invOut[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    invOut[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    invOut[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    invOut[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    invOut[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    invOut[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    invOut[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    invOut[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    invOut[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    invOut[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    invOut[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    invOut[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    invOut[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    float det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];
    if (det == 0.0f)
        return Matrix4x4::CreateScale(sf::Vector3f(1.f, 1.f, 1.f)); // Identité si non inversible

    det = 1.0f / det;
    for (int i = 0; i < 16; ++i)
        invOut[i] *= det;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            inv.m[i][j] = invOut[i * 4 + j];

    return inv;
}

// Inverse d'un quaternion (conjugué / norme au carré)
Quaternion InverseQuaternion(const Quaternion& _q)
{
    float normSq = _q.w * _q.w + _q.x * _q.x + _q.y * _q.y + _q.z * _q.z;
    if (normSq == 0.0f)
        return Quaternion{ 1.f, 0.f, 0.f, 0.f }; // Identité si non inversible
    return Quaternion{ _q.w / normSq, -_q.x / normSq, -_q.y / normSq, -_q.z / normSq };
}

float Rescale(float _value, float _min, float _max)
{
    if (_max == _min) return 0.f;
    float t = (_value - _min) / (_max - _min);
    return t;
}