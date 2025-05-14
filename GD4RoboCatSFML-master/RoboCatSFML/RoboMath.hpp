//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class Vector3
{
public:

	float		mX, mY, mZ;

	Vector3(float x, float y, float z) :
		mX(x),
		mY(y),
		mZ(z)
	{}

	Vector3() :
		mX(0.0f),
		mY(0.0f),
		mZ(0.0f)
	{}

	void Set(float x, float y, float z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}

	friend Vector3 operator+(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX + inRight.mX, inLeft.mY + inRight.mY, inLeft.mZ + inRight.mZ);
	}

	friend Vector3 operator-(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX - inRight.mX, inLeft.mY - inRight.mY, inLeft.mZ - inRight.mZ);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX * inRight.mX, inLeft.mY * inRight.mY, inLeft.mZ * inRight.mZ);
	}

	// Scalar multiply
	friend Vector3 operator*(float inScalar, const Vector3& inVec)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	friend Vector3 operator*(const Vector3& inVec, float inScalar)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	Vector3& operator*=(float inScalar)
	{
		mX *= inScalar;
		mY *= inScalar;
		mZ *= inScalar;
		return *this;
	}

	Vector3& operator+=(const Vector3& inRight)
	{
		mX += inRight.mX;
		mY += inRight.mY;
		mZ += inRight.mZ;
		return *this;
	}

	Vector3& operator-=(const Vector3& inRight)
	{
		mX -= inRight.mX;
		mY -= inRight.mY;
		mZ -= inRight.mZ;
		return *this;
	}

	float Length()
	{
		return sqrtf(mX * mX + mY * mY + mZ * mZ);
	}

	float LengthSq()
	{
		return mX * mX + mY * mY + mZ * mZ;
	}

	float Length2D()
	{
		return sqrtf(mX * mX + mY * mY);
	}

	float LengthSq2D()
	{
		return mX * mX + mY * mY;
	}

	void Normalize()
	{
		float length = Length();
		mX /= length;
		mY /= length;
		mZ /= length;
	}

	void Normalize2D()
	{
		float length = Length2D();
		mX /= length;
		mY /= length;
	}

	friend float Dot(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY + inLeft.mZ * inRight.mZ);
	}

	friend float Dot2D(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY);
	}

	friend Vector3 Cross(const Vector3& inLeft, const Vector3& inRight)
	{
		Vector3 temp;
		temp.mX = inLeft.mY * inRight.mZ - inLeft.mZ * inRight.mY;
		temp.mY = inLeft.mZ * inRight.mX - inLeft.mX * inRight.mZ;
		temp.mZ = inLeft.mX * inRight.mY - inLeft.mY * inRight.mX;
		return temp;
	}

	friend Vector3 Lerp(const Vector3& inA, const Vector3& inB, float t)
	{
		return Vector3(inA + t * (inB - inA));
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
};


class Quaternion
{
public:

	float		mX, mY, mZ, mW;

};


template< int tValue, int tBits >
struct GetRequiredBitsHelper
{
	enum { Value = GetRequiredBitsHelper< (tValue >> 1), tBits + 1 >::Value };
};

template< int tBits >
struct GetRequiredBitsHelper< 0, tBits >
{
	enum { Value = tBits };
};

template< int tValue >
struct GetRequiredBits
{
	enum { Value = GetRequiredBitsHelper< tValue, 0 >::Value };
};

namespace RoboMath
{
	const float PI = 3.1415926535f;
	float GetRandomFloat();

	Vector3 GetRandomVector(const Vector3& inMin, const Vector3& inMax);

	inline bool	Is2DVectorEqual(const Vector3& inA, const Vector3& inB)
	{
		return (inA.mX == inB.mX && inA.mY == inB.mY);
	}

	inline float ToDegrees(float inRadians)
	{
		return inRadians * 180.0f / PI;
	}

	inline float ToRadians(float inDegrees)
	{
		return (inDegrees / 180.0f) * PI;
	}
}

namespace Colors
{
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(255.0f, 255.0f, 255.0f);
	static const Vector3 Red(255.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 255.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 255.0f);
	static const Vector3 Yellow(255.0f, 255.0f, 0.0f);
	static const Vector3 Cyan(0.0f, 255.0f, 255.0f);
	static const Vector3 Magenta(255.0f, 0.0f, 255.0f);
	static const Vector3 Orange(255.0f, 165.0f, 0.0f);

	static const Vector3 Lime(50.0f, 205.0f, 50.0f);
	static const Vector3 Pink(255.0f, 192.0f, 203.0f);
	static const Vector3 Purple(128.0f, 0.0f, 128.0f);
	static const Vector3 Brown(139.0f, 69.0f, 19.0f);
	static const Vector3 SkyBlue(135.0f, 206.0f, 235.0f);
	static const Vector3 DarkGreen(0.0f, 100.0f, 0.0f);
	static const Vector3 LightGray(211.0f, 211.0f, 211.0f);
	static const Vector3 DarkGray(105.0f, 105.0f, 105.0f);
	static const Vector3 Gold(255.0f, 215.0f, 0.0f);
	static const Vector3 Navy(0.0f, 0.0f, 128.0f);
	static const Vector3 Teal(0.0f, 128.0f, 128.0f);

	static const Vector3 Salmon(250.0f, 128.0f, 114.0f);
	static const Vector3 Coral(255.0f, 127.0f, 80.0f);
	static const Vector3 Tomato(255.0f, 99.0f, 71.0f);
	static const Vector3 Olive(128.0f, 128.0f, 0.0f);
	static const Vector3 Aqua(0.0f, 255.0f, 255.0f);
	static const Vector3 Indigo(75.0f, 0.0f, 130.0f);
	static const Vector3 Plum(221.0f, 160.0f, 221.0f);
	static const Vector3 Turquoise(64.0f, 224.0f, 208.0f);
	static const Vector3 Mint(189.0f, 252.0f, 201.0f);
	static const Vector3 Lavender(230.0f, 230.0f, 250.0f);


}

