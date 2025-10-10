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
    return ABS(sqrtf(_vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z));
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
    return ABS(sqrtf(_vector.x * _vector.x + _vector.y * _vector.y));
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
    if (length != 0) {
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
    if (_len == 0) return { 1,0,0,0 };
    return { _q.w / _len, _q.x / _len, _q.y / _len, _q.z / _len };
}

Matrix4x4 MultiplyMatrix4x4(const Matrix4x4 _a, const Matrix4x4 _b)
{
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k)
                result.m[i][j] += _a.m[i][k] * _b.m[k][j];
        }
    return result;
}