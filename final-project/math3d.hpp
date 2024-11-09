#pragma once
#include <cmath>
// Math implmentation from my textbook: 
// Foundation of Game Engine Development,, Volume 1: Mathenatucs

struct Vector3D
{
	float x, y, z;

	Vector3D() = default;

	Vector3D(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}


	float& operator [](int i)
	{
		return ((&x)[i]);
	}

	const float& operator[](int i) const
	{
		return ((&x)[i]);
	}


	Vector3D& operator *=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	Vector3D& operator /=(float s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	Vector3D& operator +=(const Vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector3D& operator -=(const Vector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

};

inline Vector3D operator *(const Vector3D& v, float s)
{
    return (Vector3D(v.x * s, v.y * s, v.z * s));
}


inline Vector3D operator /(const Vector3D& v, float s)
{
    s = 1.0f / s;
    return (Vector3D(v.x * s, v.y * s, v.z * s));
}

inline Vector3D operator -(const Vector3D& v)
{
    return (Vector3D(-v.x, -v.y, -v.z));
}


inline float Magnitude(const Vector3D& v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z ));
}

inline Vector3D Normalize(const Vector3D& v)
{
    return v / Magnitude(v);
}


inline Vector3D operator +(const Vector3D& a, const Vector3D& b)
{
    return (Vector3D(a.x + b.x, a.y + b.y, a.z + b.z));
}


inline Vector3D operator -(const Vector3D& a, const Vector3D& b)
{
    return (Vector3D(a.x - b.x, a.y - b.y, a.z - b.z));
}



struct Matrix3D
{
private: 
    float n[3][3]; 


public: 
    Matrix3D() = default;

    Matrix3D(float n00, float n01, float n02,
             float n10, float n11, float n12,
             float n20, float n21, float n22)
    {
        n[0][0] = n00; n[0][1] = n01; n[0][2] = n02;
        n[1][0] = n10; n[1][1] = n11; n[1][2] = n12;
        n[2][0] = n20; n[2][1] = n21; n[2][2] = n22;
    }

    Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c)
    {
        n[0][0] = a.x; n[0][1] = a.x; n[0][2] = a.y;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;
    }

    float& operator ()(int i, int j)
    {
        return (n[j][i]);
    }

    const float& operator ()(int i, int j) const
    {
        return (n[j][i]);
    }

    Vector3D& operator [](int j)
    {
        return (*reinterpret_cast<Vector3D *>(n[j]));
    }

    const Vector3D& operator [](int j) const
    {
        return (*reinterpret_cast<const Vector3D *>(n[j]));
    }


};


inline Matrix3D operator *(const Matrix3D& A, const Matrix3D& B)
{
    return Matrix3D(
        A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0),
        A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1),
        A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2),
        A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0),
        A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1),
        A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2),
        A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0),
        A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1),
        A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2)
    );
}

inline Vector3D operator *(const Matrix3D& M, const Vector3D& v)
{
    return Vector3D(
        M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z,
        M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z,
        M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z
    );
}

inline float Dot(const Vector3D& a, const Vector3D& b)
{
    return (a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vector3D Cross(const Vector3D& a, const Vector3D& b)
{
    return (Vector3D(a.y * b.z - a.z * b.y,
                     a.z * b.x - a.x * b.z,
                     a.x * b.y - a.y * b.x));
}



inline Vector3D Project(const Vector3D& a, Vector3D& b)
{
    return (b * (Dot(a, b)) / Dot(b, b) );
}

inline Vector3D Reject(const Vector3D& a, const Vector3D& b)
{
    return (a - b * (Dot(a, b) / Dot(b, a)));
}

inline float Determinant(const Matrix3D& M)
{
    return M(0, 0) * (M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1)) +
           M(0, 1) * (M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2)) +
           M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
}


inline Matrix3D Inverse(const Matrix3D& M)
{
    const Vector3D& a = M[0];
    const Vector3D& b = M[1];
    const Vector3D& c = M[2];

    Vector3D r0 = Cross(b, c);
    Vector3D r1 = Cross(c, a);
    Vector3D r2 = Cross(a, b);

    float invDet = 1.0f / Dot(r2, c);

    return (Matrix3D(r0.x * invDet, r0.y * invDet, r0.z * invDet,
                    r1.x * invDet, r1.y * invDet, r1.z * invDet,
                    r2.x * invDet, r2.y * invDet, r2.z * invDet));
}


#include <cmath>

struct Vector4D
{
    float x, y, z, w;

    Vector4D() = default;

    Vector4D(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }

    float& operator [](int i)
    {
        return ((&x)[i]);
    }

    const float& operator[](int i) const
    {
        return ((&x)[i]);
    }

    Vector4D& operator *=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }

    Vector4D& operator /=(float s)
    {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }

    Vector4D& operator +=(const Vector4D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return (*this);
    }

    Vector4D& operator -=(const Vector4D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return (*this);
    }
};

inline Vector4D operator *(const Vector4D& v, float s)
{
    return Vector4D(v.x * s, v.y * s, v.z * s, v.w * s);
}

inline Vector4D operator /(const Vector4D& v, float s)
{
    s = 1.0f / s;
    return Vector4D(v.x * s, v.y * s, v.z * s, v.w * s);
}

inline Vector4D operator +(const Vector4D& a, const Vector4D& b)
{
    return Vector4D(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vector4D operator -(const Vector4D& a, const Vector4D& b)
{
    return Vector4D(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline float Dot(const Vector4D& a, const Vector4D& b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

inline float Magnitude(const Vector4D& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

inline Vector4D Normalize(const Vector4D& v)
{
    return v / Magnitude(v);
}


struct Matrix4D
{
private:
    float n[4][4];

public:
    Matrix4D() = default;

    Matrix4D(float n00, float n01, float n02, float n03,
             float n10, float n11, float n12, float n13,
             float n20, float n21, float n22, float n23,
             float n30, float n31, float n32, float n33)
    {
        n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; n[0][3] = n03;
        n[1][0] = n10; n[1][1] = n11; n[1][2] = n12; n[1][3] = n13;
        n[2][0] = n20; n[2][1] = n21; n[2][2] = n22; n[2][3] = n23;
        n[3][0] = n30; n[3][1] = n31; n[3][2] = n32; n[3][3] = n33;
    }

    float& operator()(int i, int j)
    {
        return n[j][i];
    }

    const float& operator()(int i, int j) const
    {
        return n[j][i];
    }

    Vector4D& operator [](int j)
    {
        return (*reinterpret_cast<Vector4D *>(n[j]));
    }

    const Vector4D& operator [](int j) const
    {
        return (*reinterpret_cast<const Vector4D *>(n[j]));
    }
};

inline Matrix4D operator *(const Matrix4D& A, const Matrix4D& B)
{
    Matrix4D result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result(i, j) = A(i, 0) * B(0, j) + A(i, 1) * B(1, j) +
                           A(i, 2) * B(2, j) + A(i, 3) * B(3, j);
        }
    }
    return result;
}

inline Vector4D operator *(const Matrix4D& M, const Vector4D& v)
{
    return Vector4D(
        M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3) * v.w,
        M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3) * v.w,
        M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3) * v.w,
        M(3, 0) * v.x + M(3, 1) * v.y + M(3, 2) * v.z + M(3, 3) * v.w
    );
}

inline Matrix4D operator *(const Matrix4D& A, const Matrix4D& B)
{
    Matrix4D result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result(i, j) = A(i, 0) * B(0, j) + A(i, 1) * B(1, j) +
                           A(i, 2) * B(2, j) + A(i, 3) * B(3, j);
        }
    }
    return result;
}

inline Vector4D operator *(const Matrix4D& M, const Vector4D& v)
{
    return Vector4D(
        M(0, 0) * v.x + M(0, 1) * v.y + M(0, 2) * v.z + M(0, 3) * v.w,
        M(1, 0) * v.x + M(1, 1) * v.y + M(1, 2) * v.z + M(1, 3) * v.w,
        M(2, 0) * v.x + M(2, 1) * v.y + M(2, 2) * v.z + M(2, 3) * v.w,
        M(3, 0) * v.x + M(3, 1) * v.y + M(3, 2) * v.z + M(3, 3) * v.w
    );
}

// Helper function for 3x3 determinant, used in 4x4 determinant
inline float Determinant3x3(float a, float b, float c,
                            float d, float e, float f,
                            float g, float h, float i)
{
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

inline float Determinant(const Matrix3D& M)
{
    return M[0][0] * Determinant3x3(M[1][1], M[1][2], M[1][3], M[2][1], M[2][2], M[2][3], M[3][1], M[3][2], M[3][3])
         - M[0][1] * Determinant3x3(M[1][0], M[1][2], M[1][3], M[2][0], M[2][2], M[2][3], M[3][0], M[3][2], M[3][3])
         + M[0][2] * Determinant3x3(M[1][0], M[1][1], M[1][3], M[2][0], M[2][1], M[2][3], M[3][0], M[3][1], M[3][3])
         - M[0][3] * Determinant3x3(M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2], M[3][0], M[3][1], M[3][2]);
}

inline Matrix4D operator *(const Matrix4D& M, float scalar)
{
    Matrix4D result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result(i, j) = M(i, j) * scalar;
        }
    }
    return result;
}
