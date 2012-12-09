#ifndef VECTOR_H
#define VECTOR_H


class Vector
{
    public:
        Vector();
        Vector(double x, double y);
        Vector(const Vector& v);

        virtual ~Vector();

        /// Add a vector v to this vector, and return the results.
        Vector& Add(const Vector& v);

        /// Subtract a vector v from this vector, and return the results.
        Vector& Subtract(const Vector& v);

        /// Multiply both components by scalar.
        Vector& Scale(double scalar);

        //Elementwise multiplication
        Vector& Multiply(const Vector& v);

        /// Calculates the dot product of this vector and v.
        double GetDotProduct(const Vector& v) const;

        Vector& Normalize();

        void Set(double x, double y);
        void Set(const Vector& v);

        Vector& operator=(const Vector& v);
        double& operator[] (unsigned int key);

        Vector operator +(const Vector& v) const;
        Vector operator -(const Vector& v) const;
        Vector operator *(double scalar) const;
        double operator *(const Vector& v) const;
        Vector operator /(double scalar) const;
        Vector operator-() const;

        bool operator==(const Vector& v) const;
        bool operator!=(const Vector& v) const;

        double getLength() const;

        Vector getNormalized() const;

    protected:
    private:
        double x[2];
};

#endif // VECTOR_H
