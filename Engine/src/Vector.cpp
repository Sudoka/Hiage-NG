#include <cmath>
#include "Vector.h"

Vector::Vector()
{
    x[0] = 0;
    x[1] = 0;
}

Vector::Vector(double x, double y)
{
    this->x[0] = x; 
    this->x[1] = y; 
}

Vector::~Vector()
{
}

/// <summary>
/// Add a vector v to this vector, and return the results.
/// </summary>
/// <param name="v">
/// A <see cref="Vector"/>
/// </param>
/// <returns>
/// The resulting <see cref="Vector"/>
/// </returns>
Vector& Vector::Add(const Vector& v)
{

    x[0] += v.x[0];
    x[1] += v.x[1];

    return *this;
}

/// <summary>
/// Subtract a vector v from this vector, and return the results.
/// </summary>
/// <param name="v">
/// A <see cref="Vector"/>
/// </param>
/// <returns>
/// A <see cref="Vector"/>
/// </returns>
Vector& Vector::Subtract(const Vector& v)
{
    x[0] -= v.x[0];
    x[1] -= v.x[1];

    return *this;
}

/// <summary>
/// Multiply both components by scalar.
/// </summary>
/// <param name="scalar">
/// A <see cref="System.Double"/>
/// </param>
/// <returns>
/// The resulting <see cref="Vector"/>
/// </returns>
Vector& Vector::Scale(double scalar)
{
    x[0] *= scalar;
    x[1] *= scalar;

    return *this;
}

//Elementwise multiplication
Vector& Vector::Multiply(const Vector& v)
{
    x[0] *= v.x[0];
    x[1] *= v.x[1];

    return *this;
}

/// <summary>
/// Calculates the dot product of this vector and v.
/// </summary>
/// <param name="v">
/// A <see cref="Vector"/>
/// </param>
/// <returns>
/// The dot product (or scalar product).
/// </returns>
double Vector::GetDotProduct(const Vector& v) const
{
    return (x[0] * v.x[0] + x[1] * v.x[1]);
}

Vector& Vector::Normalize()
{
    double length = getLength();
    if (length > 0)
        Scale(1/length);

    return *this;
}

Vector& Vector::operator=(const Vector& v)
{
    x[0] = v.x[0];
    x[1] = v.x[1];

    return *this;
}

double& Vector::operator[] (unsigned int key) 
{
    return x[key];
}

Vector Vector::operator +(const Vector& v) const
{
    return Vector(x[0]+v.x[0], x[1]+v.x[1]);
}

Vector Vector::operator -(const Vector& v) const
{
    return Vector(x[0]-v.x[0], x[1]-v.x[1]);
}

Vector Vector::operator *(double scalar) const
{
    return Vector(x[0] * scalar, x[1] * scalar);
}

double Vector::operator *(const Vector& v) const
{
    return x[0]*v.x[0]+x[1]*v.x[1];
}

Vector Vector::operator /(double scalar) const
{
    return Vector(x[0] / scalar, x[1] / scalar);
}

Vector Vector::operator-() const
{
    return Vector(-x[0], -x[1]);
}

 bool Vector::operator==(const Vector& v) const
{
    return x[0] == v.x[0] && x[1] == v.x[1];
}

bool Vector::operator!=(const Vector& v) const
{
    return !(this->operator==(v));
}

double Vector::getLength() const
{
    return sqrt(x[0]*x[0]+x[1]*x[1]);
}

Vector Vector::getNormalized() const
{
    return Vector(*this).Normalize();
}

