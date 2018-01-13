#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>
#include <limits>
namespace des {
	template<typename T> class Vec3;

template<typename T>
class Vec2
{
public:
	T x, y;

	typedef T							TYPE;
	typedef T							value_type;

	static const int DIM = 2;

	Vec2() :x(0), y(0) {}
	Vec2(T nx, T ny) : x(nx), y(ny) {}
	Vec2(const Vec2<T>& src) : x(src.x), y(src.y) {}
	explicit Vec2(const T *d) : x(d[0]), y(d[1]) {}

	template<typename FromT>
	Vec2(const Vec2<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y))
	{}

	void set(T ax, T ay)
	{
		x = ax; y = ay;
	}

	void set(const Vec2<T> &rhs)
	{
		x = rhs.x; y = rhs.y;
	}

	// Operators
	template<typename FromT>
	Vec2<T>& operator=(const Vec2<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		return *this;
	}

	Vec2<T>& operator=(const Vec2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	T& operator[](int n)
	{
		assert(n >= 0 && n <= 1);
		return (&x)[n];
	}

	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 1);
		return (&x)[n];
	}

	T*	ptr() const { return &(const_cast<Vec2*>(this)->x); }

	const Vec2<T>	operator+(const Vec2<T>& rhs) const { return Vec2<T>(x + rhs.x, y + rhs.y); }
	const Vec2<T>	operator-(const Vec2<T>& rhs) const { return Vec2<T>(x - rhs.x, y - rhs.y); }
	const Vec2<T>	operator*(const Vec2<T>& rhs) const { return Vec2<T>(x * rhs.x, y * rhs.y); }
	const Vec2<T>	operator/(const Vec2<T>& rhs) const { return Vec2<T>(x / rhs.x, y / rhs.y); }
	Vec2<T>&	operator+=(const Vec2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vec2<T>&	operator-=(const Vec2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Vec2<T>&	operator*=(const Vec2<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	Vec2<T>&	operator/=(const Vec2<T>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
	const Vec2<T>	operator/(T rhs) const { return Vec2<T>(x / rhs, y / rhs); }
	Vec2<T>&	operator+=(T rhs) { x += rhs;	y += rhs; return *this; }
	Vec2<T>&	operator-=(T rhs) { x -= rhs; y -= rhs; return *this; }
	Vec2<T>&	operator*=(T rhs) { x *= rhs; y *= rhs; return *this; }
	Vec2<T>&	operator/=(T rhs) { x /= rhs; y /= rhs; return *this; }

	Vec2<T>		operator-() const { return Vec2<T>(-x, -y); } // unary negation

	bool operator==(const Vec2<T> &rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const Vec2<T> &rhs) const
	{
		return !(*this == rhs);
	}

	T dot(const Vec2<T> &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	//! Returns the z component of the cross if the two operands were Vec3's on the XY plane, the equivalent of Vec3(*this).cross( Vec3(rhs) ).z
	T cross(const Vec2<T> &rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}



	T distanceSquared(const Vec2<T> &rhs) const
	{
		return (*this - rhs).lengthSquared();
	}



	

	

	

	

	T lengthSquared() const
	{
		return x * x + y * y;
	}



	

	void invert()
	{
		x = -x;
		y = -y;
	}

	Vec2<T> inverse() const
	{
		return Vec2<T>(-x, -y);
	}

	Vec2<T> lerp(T fact, const Vec2<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	void lerpEq(T fact, const Vec2<T> &rhs)
	{
		x = x + (rhs.x - x) * fact; y = y + (rhs.y - y) * fact;
	}

	// GLSL inspired swizzling functions.
	Vec2<T> xx() const { return Vec2<T>(x, x); }
	Vec2<T> xy() const { return Vec2<T>(x, y); }
	Vec2<T> yx() const { return Vec2<T>(y, x); }
	Vec2<T> yy() const { return Vec2<T>(y, y); }

	Vec3<T> xxx() const { return Vec3<T>(x, x, x); }
	Vec3<T> xxy() const { return Vec3<T>(x, x, y); }
	Vec3<T> xyx() const { return Vec3<T>(x, y, x); }
	Vec3<T> xyy() const { return Vec3<T>(x, y, y); }
	Vec3<T> yxx() const { return Vec3<T>(y, x, x); }
	Vec3<T> yxy() const { return Vec3<T>(y, x, y); }
	Vec3<T> yyx() const { return Vec3<T>(y, y, x); }
	Vec3<T> yyy() const { return Vec3<T>(y, y, y); }

	static Vec2<T> max()
	{
		return Vec2<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
	}

	static Vec2<T> zero()
	{
		return Vec2<T>(0, 0);
	}

	static Vec2<T> one()
	{
		return Vec2<T>(1, 1);
	}

	friend std::ostream& operator<<(std::ostream& lhs, const Vec2<T>& rhs)
	{
		lhs << "[" << rhs.x << "," << rhs.y << "]";
		return lhs;
	}

	static Vec2<T> xAxis() { return Vec2<T>(1, 0); }
	static Vec2<T> yAxis() { return Vec2<T>(0, 1); }

	
	
};

template<typename T>
class Vec3
{
public:
	T x, y, z;

	typedef T								TYPE;
	typedef T								value_type;
	static const int DIM = 3;

	Vec3() :x(0), y(0), z(0) {}
	Vec3(T nx, T ny, T nz)
		: x(nx), y(ny), z(nz)
	{}
	Vec3(const Vec3<T> &src)
		: x(src.x), y(src.y), z(src.z)
	{}
	Vec3(const Vec2<T> &v2, T aZ)
		: x(v2.x), y(v2.y), z(aZ)
	{}
	explicit Vec3(const Vec2<T> &v2)
		: x(v2.x), y(v2.y), z(0)
	{}
	explicit Vec3(const T *d) : x(d[0]), y(d[1]), z(d[2]) {}
	template<typename FromT>
	Vec3(const Vec3<FromT> &src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z))
	{}
	template<typename Y>
	

	void set(T ax, T ay, T az)
	{
		x = ax; y = ay; z = az;
	}

	void set(const Vec3<T> &rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
	}

	Vec3<T>& operator=(const Vec3<T> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	template<typename FromT>
	Vec3<T>& operator=(const Vec3<FromT> &rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		return *this;
	}

	T& operator[](int n)
	{
		assert(n >= 0 && n <= 2);
		return (&x)[n];
	}

	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 2);
		return (&x)[n];
	}

	T*	ptr() const { return &(const_cast<Vec3*>(this)->x); }

	const Vec3<T>	operator+(const Vec3<T>& rhs) const { return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
	const Vec3<T>	operator-(const Vec3<T>& rhs) const { return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
	const Vec3<T>	operator*(const Vec3<T>& rhs) const { return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z); }
	const Vec3<T>	operator/(const Vec3<T>& rhs) const { return Vec3<T>(x / rhs.x, y / rhs.y, z / rhs.z); }
	Vec3<T>&	operator+=(const Vec3<T>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vec3<T>&	operator-=(const Vec3<T>& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vec3<T>&	operator*=(const Vec3<T>& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	Vec3<T>&	operator/=(const Vec3<T>& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }
	const Vec3<T>	operator/(T rhs) const { T invRhs = static_cast<T>(1.0) / rhs; return Vec3<T>(x * invRhs, y * invRhs, z * invRhs); }
	Vec3<T>&	operator+=(T rhs) { x += rhs; y += rhs; z += rhs; return *this; }
	Vec3<T>&	operator-=(T rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
	Vec3<T>&	operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
	Vec3<T>&	operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

	Vec3<T>		operator-() const { return Vec3<T>(-x, -y, -z); } // unary negation

	bool operator==(const Vec3<T>& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	bool operator!=(const Vec3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	T dot(const Vec3<T> &rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	Vec3<T> cross(const Vec3<T> &rhs) const
	{
		return Vec3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
	}

	T distance(const Vec3<T> &rhs) const
	{
		return (*this - rhs).length();
	}

	T distanceSquared(const Vec3<T> &rhs) const
	{
		return (*this - rhs).lengthSquared();
	}

	

	T lengthSquared() const
	{
		return x*x + y*y + z*z;
	}

	

	

	void invert()
	{
		x = -x; y = -y; z = -z;
	}

	Vec3<T> inverse() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	

	

	

	Vec3<T> lerp(T fact, const Vec3<T> &rhs) const
	{
		return (*this) + (rhs - (*this)) * fact;
	}

	void lerpEq(T fact, const Vec3<T> &rhs)
	{
		x = x + (rhs.x - x) * fact; y = y + (rhs.y - y) * fact; z = z + (rhs.z - z) * fact;
	}

	static Vec3<T> max()
	{
		return Vec3<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
	}

	static Vec3<T> zero()
	{
		return Vec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
	}

	static Vec3<T> one()
	{
		return Vec3<T>(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));
	}

	

	// derived from but not equivalent to Quaternion::squad
	Vec3<T> squad(T t, const Vec3<T> &tangentA, const Vec3<T> &tangentB, const Vec3<T> &end) const
	{
		Vec3<T> r1 = this->slerp(t, end);
		Vec3<T> r2 = tangentA.slerp(t, tangentB);
		return r1.slerp(2 * t * (1 - t), r2);
	}

	// GLSL inspired swizzling functions.
	Vec2<T> xx() const { return Vec2<T>(x, x); }
	Vec2<T> xy() const { return Vec2<T>(x, y); }
	Vec2<T> xz() const { return Vec2<T>(x, z); }
	Vec2<T> yx() const { return Vec2<T>(y, x); }
	Vec2<T> yy() const { return Vec2<T>(y, y); }
	Vec2<T> yz() const { return Vec2<T>(y, z); }
	Vec2<T> zx() const { return Vec2<T>(z, x); }
	Vec2<T> zy() const { return Vec2<T>(z, y); }
	Vec2<T> zz() const { return Vec2<T>(z, z); }

	Vec3<T> xxx() const { return Vec3<T>(x, x, x); }
	Vec3<T> xxy() const { return Vec3<T>(x, x, y); }
	Vec3<T> xxz() const { return Vec3<T>(x, x, z); }
	Vec3<T> xyx() const { return Vec3<T>(x, y, x); }
	Vec3<T> xyy() const { return Vec3<T>(x, y, y); }
	Vec3<T> xyz() const { return Vec3<T>(x, y, z); }
	Vec3<T> xzx() const { return Vec3<T>(x, z, x); }
	Vec3<T> xzy() const { return Vec3<T>(x, z, y); }
	Vec3<T> xzz() const { return Vec3<T>(x, z, z); }
	Vec3<T> yxx() const { return Vec3<T>(y, x, x); }
	Vec3<T> yxy() const { return Vec3<T>(y, x, y); }
	Vec3<T> yxz() const { return Vec3<T>(y, x, z); }
	Vec3<T> yyx() const { return Vec3<T>(y, y, x); }
	Vec3<T> yyy() const { return Vec3<T>(y, y, y); }
	Vec3<T> yyz() const { return Vec3<T>(y, y, z); }
	Vec3<T> yzx() const { return Vec3<T>(y, z, x); }
	Vec3<T> yzy() const { return Vec3<T>(y, z, y); }
	Vec3<T> yzz() const { return Vec3<T>(y, z, z); }
	Vec3<T> zxx() const { return Vec3<T>(z, x, x); }
	Vec3<T> zxy() const { return Vec3<T>(z, x, y); }
	Vec3<T> zxz() const { return Vec3<T>(z, x, z); }
	Vec3<T> zyx() const { return Vec3<T>(z, y, x); }
	Vec3<T> zyy() const { return Vec3<T>(z, y, y); }
	Vec3<T> zyz() const { return Vec3<T>(z, y, z); }
	Vec3<T> zzx() const { return Vec3<T>(z, z, x); }
	Vec3<T> zzy() const { return Vec3<T>(z, z, y); }
	Vec3<T> zzz() const { return Vec3<T>(z, z, z); }

	friend std::ostream& operator<<(std::ostream& lhs, const Vec3<T> rhs)
	{
		lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
		return lhs;
	}

	static Vec3<T> xAxis() { return Vec3<T>(1, 0, 0); }
	static Vec3<T> yAxis() { return Vec3<T>(0, 1, 0); }
	static Vec3<T> zAxis() { return Vec3<T>(0, 0, 1); }

	
};

template <class T>
class Vec4
{
public:
	T x, y, z, w;

	typedef T							TYPE;
	typedef T							value_type;
	static const int DIM = 4;

	Vec4()
		: x(0), y(0), z(0), w(0)
	{}
	Vec4(T nx, T ny, T nz, T nw = 0)
		: x(nx), y(ny), z(nz), w(nw)
	{}
	Vec4(const Vec3<T>& src, T aW = 0)
		: x(src.x), y(src.y), z(src.z), w(aW)
	{}
	Vec4(const Vec4<T>& src)
		: x(src.x), y(src.y), z(src.z), w(src.w)
	{}
	template<typename FromT>
	Vec4(const Vec4<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w))
	{}
	explicit Vec4(const T *d) : x(d[0]), y(d[1]), z(d[2]), w(d[3]) {}

	void set(T ax, T ay, T az, T aw)
	{
		x = ax; y = ay; z = az; w = aw;
	}

	void set(const Vec4<T> &rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
	}

	Vec4<T>& operator=(const Vec4<T>& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		return *this;
	}

	template<typename FromT>
	Vec4<T>& operator=(const Vec4<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x); y = static_cast<T>(rhs.y); z = static_cast<T>(rhs.z); w = static_cast<T>(rhs.w);
		return *this;
	}

	T& operator[](int n)
	{
		assert(n >= 0 && n <= 3);
		return (&x)[n];
	}

	const T& operator[](int n)  const
	{
		assert(n >= 0 && n <= 3);
		return (&x)[n];
	}

	T*	ptr() const { return &(const_cast<Vec4*>(this)->x); }

	const Vec4<T>	operator+(const Vec4<T>& rhs) const { return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	const Vec4<T>	operator-(const Vec4<T>& rhs) const { return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	const Vec4<T>	operator*(const Vec4<T>& rhs) const { return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	const Vec4<T>	operator/(const Vec4<T>& rhs) const { return Vec4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	Vec4<T>&		operator+=(const Vec4<T>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Vec4<T>&		operator-=(const Vec4<T>& rhs) { x -= rhs.x;	y -= rhs.y;	z -= rhs.z;	w -= rhs.w;	return *this; }
	Vec4<T>&		operator*=(const Vec4<T>& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z;	w *= rhs.w;	return *this; }
	Vec4<T>&		operator/=(const Vec4<T>& rhs) { x /= rhs.x;	y /= rhs.y;	z /= rhs.z;	w /= rhs.w;	return *this; }
	const Vec4<T>	operator/(T rhs) const { return Vec4<T>(x / rhs, y / rhs, z / rhs, w / rhs); }
	Vec4<T>&		operator+=(T rhs) { x += rhs; y += rhs; z += rhs; w += rhs; return *this; }
	Vec4<T>&		operator-=(T rhs) { x -= rhs; y -= rhs; z -= rhs; w -= rhs;	return *this; }
	Vec4<T>&		operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	Vec4<T>&		operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs;	return *this; }

	Vec4<T>			operator-() const { return Vec4<T>(-x, -y, -z, -w); } // unary negation

	bool operator==(const Vec4<T>& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
	}

	bool operator!=(const Vec4<T>& rhs) const
	{
		return !(*this == rhs);
	}

	T dot(const Vec4<T> &rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	Vec4<T> cross(const Vec4<T> &rhs) const
	{
		return Vec4<T>(y*rhs.z - rhs.y*z, z*rhs.x - rhs.z*x, x*rhs.y - rhs.x*y);
	}

	T distance(const Vec4<T> &rhs) const
	{
		return (*this - rhs).length();
	}

	T distanceSquared(const Vec4<T> &rhs) const
	{
		return (*this - rhs).lengthSquared();
	}

	

	T lengthSquared() const
	{
		// For most vector operations, this assumes w to be zero.
		return x*x + y*y + z*z + w*w;
	}

	

	//! Returns a copy of the Vec4 with its length limited to \a maxLength, scaling it proportionally if necessary.
	Vec4<T> limited(T maxLength) const
	{
		T lenSq = lengthSquared();

		

		/*
		T lengthSquared = x * x + y * y + z * z + w * w;

		if( ( lengthSquared > maxLength * maxLength ) && ( lengthSquared > 0 ) ) {
		T ratio = maxLength / math<T>::sqrt( lengthSquared );
		return Vec4<T>( x * ratio, y * ratio, z * ratio, w * ratio );
		}
		else
		return *this;
		*/
	}

	void invert()
	{
		x = -x; y = -y; z = -z; w = -w;
	}

	Vec4<T> inverse() const
	{
		return Vec4<T>(-x, -y, -z, -w);
	}

	Vec4<T> lerp(T fact, const Vec4<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	void lerpEq(T fact, const Vec4<T> &rhs)
	{
		x = x + (rhs.x - x) * fact; y = y + (rhs.y - y) * fact; z = z + (rhs.z - z) * fact; w = w + (rhs.w - w) * fact;
	}

	static Vec4<T> max()
	{
		return Vec4<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
	}

	static Vec4<T> zero()
	{
		return Vec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
	}

	static Vec4<T> one()
	{
		return Vec4<T>(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));
	}

	

	// derived from but not equivalent to Quaternion::squad
	Vec4<T> squad(T t, const Vec4<T> &tangentA, const Vec4<T> &tangentB, const Vec4<T> &end) const
	{
		Vec4<T> r1 = this->slerp(t, end);
		Vec4<T> r2 = tangentA.slerp(t, tangentB);
		return r1.slerp(2 * t * (1 - t), r2);
	}

	// GLSL inspired swizzling functions.
	Vec2<T> xx() const { return Vec2<T>(x, x); }
	Vec2<T> xy() const { return Vec2<T>(x, y); }
	Vec2<T> xz() const { return Vec2<T>(x, z); }
	Vec2<T> yx() const { return Vec2<T>(y, x); }
	Vec2<T> yy() const { return Vec2<T>(y, y); }
	Vec2<T> yz() const { return Vec2<T>(y, z); }
	Vec2<T> zx() const { return Vec2<T>(z, x); }
	Vec2<T> zy() const { return Vec2<T>(z, y); }
	Vec2<T> zz() const { return Vec2<T>(z, z); }

	Vec3<T> xxx() const { return Vec3<T>(x, x, x); }
	Vec3<T> xxy() const { return Vec3<T>(x, x, y); }
	Vec3<T> xxz() const { return Vec3<T>(x, x, z); }
	Vec3<T> xyx() const { return Vec3<T>(x, y, x); }
	Vec3<T> xyy() const { return Vec3<T>(x, y, y); }
	Vec3<T> xyz() const { return Vec3<T>(x, y, z); }
	Vec3<T> xzx() const { return Vec3<T>(x, z, x); }
	Vec3<T> xzy() const { return Vec3<T>(x, z, y); }
	Vec3<T> xzz() const { return Vec3<T>(x, z, z); }
	Vec3<T> yxx() const { return Vec3<T>(y, x, x); }
	Vec3<T> yxy() const { return Vec3<T>(y, x, y); }
	Vec3<T> yxz() const { return Vec3<T>(y, x, z); }
	Vec3<T> yyx() const { return Vec3<T>(y, y, x); }
	Vec3<T> yyy() const { return Vec3<T>(y, y, y); }
	Vec3<T> yyz() const { return Vec3<T>(y, y, z); }
	Vec3<T> yzx() const { return Vec3<T>(y, z, x); }
	Vec3<T> yzy() const { return Vec3<T>(y, z, y); }
	Vec3<T> yzz() const { return Vec3<T>(y, z, z); }
	Vec3<T> zxx() const { return Vec3<T>(z, x, x); }
	Vec3<T> zxy() const { return Vec3<T>(z, x, y); }
	Vec3<T> zxz() const { return Vec3<T>(z, x, z); }
	Vec3<T> zyx() const { return Vec3<T>(z, y, x); }
	Vec3<T> zyy() const { return Vec3<T>(z, y, y); }
	Vec3<T> zyz() const { return Vec3<T>(z, y, z); }
	Vec3<T> zzx() const { return Vec3<T>(z, z, x); }
	Vec3<T> zzy() const { return Vec3<T>(z, z, y); }
	Vec3<T> zzz() const { return Vec3<T>(z, z, z); }

	Vec4<T> xxxx() const { return Vec4<T>(x, x, x, x); }
	Vec4<T> xxxy() const { return Vec4<T>(x, x, x, y); }
	Vec4<T> xxxz() const { return Vec4<T>(x, x, x, z); }
	Vec4<T> xxxw() const { return Vec4<T>(x, x, x, w); }
	Vec4<T> xxyx() const { return Vec4<T>(x, x, y, x); }
	Vec4<T> xxyy() const { return Vec4<T>(x, x, y, y); }
	Vec4<T> xxyz() const { return Vec4<T>(x, x, y, z); }
	Vec4<T> xxyw() const { return Vec4<T>(x, x, y, w); }
	Vec4<T> xxzx() const { return Vec4<T>(x, x, z, x); }
	Vec4<T> xxzy() const { return Vec4<T>(x, x, z, y); }
	Vec4<T> xxzz() const { return Vec4<T>(x, x, z, z); }
	Vec4<T> xxzw() const { return Vec4<T>(x, x, z, w); }
	Vec4<T> xxwx() const { return Vec4<T>(x, x, w, x); }
	Vec4<T> xxwy() const { return Vec4<T>(x, x, w, y); }
	Vec4<T> xxwz() const { return Vec4<T>(x, x, w, z); }
	Vec4<T> xxww() const { return Vec4<T>(x, x, w, w); }
	Vec4<T> xyxx() const { return Vec4<T>(x, y, x, x); }
	Vec4<T> xyxy() const { return Vec4<T>(x, y, x, y); }
	Vec4<T> xyxz() const { return Vec4<T>(x, y, x, z); }
	Vec4<T> xyxw() const { return Vec4<T>(x, y, x, w); }
	Vec4<T> xyyx() const { return Vec4<T>(x, y, y, x); }
	Vec4<T> xyyy() const { return Vec4<T>(x, y, y, y); }
	Vec4<T> xyyz() const { return Vec4<T>(x, y, y, z); }
	Vec4<T> xyyw() const { return Vec4<T>(x, y, y, w); }
	Vec4<T> xyzx() const { return Vec4<T>(x, y, z, x); }
	Vec4<T> xyzy() const { return Vec4<T>(x, y, z, y); }
	Vec4<T> xyzz() const { return Vec4<T>(x, y, z, z); }
	Vec4<T> xyzw() const { return Vec4<T>(x, y, z, w); }
	Vec4<T> xywx() const { return Vec4<T>(x, y, w, x); }
	Vec4<T> xywy() const { return Vec4<T>(x, y, w, y); }
	Vec4<T> xywz() const { return Vec4<T>(x, y, w, z); }
	Vec4<T> xyww() const { return Vec4<T>(x, y, w, w); }
	Vec4<T> xzxx() const { return Vec4<T>(x, z, x, x); }
	Vec4<T> xzxy() const { return Vec4<T>(x, z, x, y); }
	Vec4<T> xzxz() const { return Vec4<T>(x, z, x, z); }
	Vec4<T> xzxw() const { return Vec4<T>(x, z, x, w); }
	Vec4<T> xzyx() const { return Vec4<T>(x, z, y, x); }
	Vec4<T> xzyy() const { return Vec4<T>(x, z, y, y); }
	Vec4<T> xzyz() const { return Vec4<T>(x, z, y, z); }
	Vec4<T> xzyw() const { return Vec4<T>(x, z, y, w); }
	Vec4<T> xzzx() const { return Vec4<T>(x, z, z, x); }
	Vec4<T> xzzy() const { return Vec4<T>(x, z, z, y); }
	Vec4<T> xzzz() const { return Vec4<T>(x, z, z, z); }
	Vec4<T> xzzw() const { return Vec4<T>(x, z, z, w); }
	Vec4<T> xzwx() const { return Vec4<T>(x, z, w, x); }
	Vec4<T> xzwy() const { return Vec4<T>(x, z, w, y); }
	Vec4<T> xzwz() const { return Vec4<T>(x, z, w, z); }
	Vec4<T> xzww() const { return Vec4<T>(x, z, w, w); }
	Vec4<T> xwxx() const { return Vec4<T>(x, w, x, x); }
	Vec4<T> xwxy() const { return Vec4<T>(x, w, x, y); }
	Vec4<T> xwxz() const { return Vec4<T>(x, w, x, z); }
	Vec4<T> xwxw() const { return Vec4<T>(x, w, x, w); }
	Vec4<T> xwyx() const { return Vec4<T>(x, w, y, x); }
	Vec4<T> xwyy() const { return Vec4<T>(x, w, y, y); }
	Vec4<T> xwyz() const { return Vec4<T>(x, w, y, z); }
	Vec4<T> xwyw() const { return Vec4<T>(x, w, y, w); }
	Vec4<T> xwzx() const { return Vec4<T>(x, w, z, x); }
	Vec4<T> xwzy() const { return Vec4<T>(x, w, z, y); }
	Vec4<T> xwzz() const { return Vec4<T>(x, w, z, z); }
	Vec4<T> xwzw() const { return Vec4<T>(x, w, z, w); }
	Vec4<T> xwwx() const { return Vec4<T>(x, w, w, x); }
	Vec4<T> xwwy() const { return Vec4<T>(x, w, w, y); }
	Vec4<T> xwwz() const { return Vec4<T>(x, w, w, z); }
	Vec4<T> xwww() const { return Vec4<T>(x, w, w, w); }
	Vec4<T> yxxx() const { return Vec4<T>(y, x, x, x); }
	Vec4<T> yxxy() const { return Vec4<T>(y, x, x, y); }
	Vec4<T> yxxz() const { return Vec4<T>(y, x, x, z); }
	Vec4<T> yxxw() const { return Vec4<T>(y, x, x, w); }
	Vec4<T> yxyx() const { return Vec4<T>(y, x, y, x); }
	Vec4<T> yxyy() const { return Vec4<T>(y, x, y, y); }
	Vec4<T> yxyz() const { return Vec4<T>(y, x, y, z); }
	Vec4<T> yxyw() const { return Vec4<T>(y, x, y, w); }
	Vec4<T> yxzx() const { return Vec4<T>(y, x, z, x); }
	Vec4<T> yxzy() const { return Vec4<T>(y, x, z, y); }
	Vec4<T> yxzz() const { return Vec4<T>(y, x, z, z); }
	Vec4<T> yxzw() const { return Vec4<T>(y, x, z, w); }
	Vec4<T> yxwx() const { return Vec4<T>(y, x, w, x); }
	Vec4<T> yxwy() const { return Vec4<T>(y, x, w, y); }
	Vec4<T> yxwz() const { return Vec4<T>(y, x, w, z); }
	Vec4<T> yxww() const { return Vec4<T>(y, x, w, w); }
	Vec4<T> yyxx() const { return Vec4<T>(y, y, x, x); }
	Vec4<T> yyxy() const { return Vec4<T>(y, y, x, y); }
	Vec4<T> yyxz() const { return Vec4<T>(y, y, x, z); }
	Vec4<T> yyxw() const { return Vec4<T>(y, y, x, w); }
	Vec4<T> yyyx() const { return Vec4<T>(y, y, y, x); }
	Vec4<T> yyyy() const { return Vec4<T>(y, y, y, y); }
	Vec4<T> yyyz() const { return Vec4<T>(y, y, y, z); }
	Vec4<T> yyyw() const { return Vec4<T>(y, y, y, w); }
	Vec4<T> yyzx() const { return Vec4<T>(y, y, z, x); }
	Vec4<T> yyzy() const { return Vec4<T>(y, y, z, y); }
	Vec4<T> yyzz() const { return Vec4<T>(y, y, z, z); }
	Vec4<T> yyzw() const { return Vec4<T>(y, y, z, w); }
	Vec4<T> yywx() const { return Vec4<T>(y, y, w, x); }
	Vec4<T> yywy() const { return Vec4<T>(y, y, w, y); }
	Vec4<T> yywz() const { return Vec4<T>(y, y, w, z); }
	Vec4<T> yyww() const { return Vec4<T>(y, y, w, w); }
	Vec4<T> yzxx() const { return Vec4<T>(y, z, x, x); }
	Vec4<T> yzxy() const { return Vec4<T>(y, z, x, y); }
	Vec4<T> yzxz() const { return Vec4<T>(y, z, x, z); }
	Vec4<T> yzxw() const { return Vec4<T>(y, z, x, w); }
	Vec4<T> yzyx() const { return Vec4<T>(y, z, y, x); }
	Vec4<T> yzyy() const { return Vec4<T>(y, z, y, y); }
	Vec4<T> yzyz() const { return Vec4<T>(y, z, y, z); }
	Vec4<T> yzyw() const { return Vec4<T>(y, z, y, w); }
	Vec4<T> yzzx() const { return Vec4<T>(y, z, z, x); }
	Vec4<T> yzzy() const { return Vec4<T>(y, z, z, y); }
	Vec4<T> yzzz() const { return Vec4<T>(y, z, z, z); }
	Vec4<T> yzzw() const { return Vec4<T>(y, z, z, w); }
	Vec4<T> yzwx() const { return Vec4<T>(y, z, w, x); }
	Vec4<T> yzwy() const { return Vec4<T>(y, z, w, y); }
	Vec4<T> yzwz() const { return Vec4<T>(y, z, w, z); }
	Vec4<T> yzww() const { return Vec4<T>(y, z, w, w); }
	Vec4<T> ywxx() const { return Vec4<T>(y, w, x, x); }
	Vec4<T> ywxy() const { return Vec4<T>(y, w, x, y); }
	Vec4<T> ywxz() const { return Vec4<T>(y, w, x, z); }
	Vec4<T> ywxw() const { return Vec4<T>(y, w, x, w); }
	Vec4<T> ywyx() const { return Vec4<T>(y, w, y, x); }
	Vec4<T> ywyy() const { return Vec4<T>(y, w, y, y); }
	Vec4<T> ywyz() const { return Vec4<T>(y, w, y, z); }
	Vec4<T> ywyw() const { return Vec4<T>(y, w, y, w); }
	Vec4<T> ywzx() const { return Vec4<T>(y, w, z, x); }
	Vec4<T> ywzy() const { return Vec4<T>(y, w, z, y); }
	Vec4<T> ywzz() const { return Vec4<T>(y, w, z, z); }
	Vec4<T> ywzw() const { return Vec4<T>(y, w, z, w); }
	Vec4<T> ywwx() const { return Vec4<T>(y, w, w, x); }
	Vec4<T> ywwy() const { return Vec4<T>(y, w, w, y); }
	Vec4<T> ywwz() const { return Vec4<T>(y, w, w, z); }
	Vec4<T> ywww() const { return Vec4<T>(y, w, w, w); }
	Vec4<T> zxxx() const { return Vec4<T>(z, x, x, x); }
	Vec4<T> zxxy() const { return Vec4<T>(z, x, x, y); }
	Vec4<T> zxxz() const { return Vec4<T>(z, x, x, z); }
	Vec4<T> zxxw() const { return Vec4<T>(z, x, x, w); }
	Vec4<T> zxyx() const { return Vec4<T>(z, x, y, x); }
	Vec4<T> zxyy() const { return Vec4<T>(z, x, y, y); }
	Vec4<T> zxyz() const { return Vec4<T>(z, x, y, z); }
	Vec4<T> zxyw() const { return Vec4<T>(z, x, y, w); }
	Vec4<T> zxzx() const { return Vec4<T>(z, x, z, x); }
	Vec4<T> zxzy() const { return Vec4<T>(z, x, z, y); }
	Vec4<T> zxzz() const { return Vec4<T>(z, x, z, z); }
	Vec4<T> zxzw() const { return Vec4<T>(z, x, z, w); }
	Vec4<T> zxwx() const { return Vec4<T>(z, x, w, x); }
	Vec4<T> zxwy() const { return Vec4<T>(z, x, w, y); }
	Vec4<T> zxwz() const { return Vec4<T>(z, x, w, z); }
	Vec4<T> zxww() const { return Vec4<T>(z, x, w, w); }
	Vec4<T> zyxx() const { return Vec4<T>(z, y, x, x); }
	Vec4<T> zyxy() const { return Vec4<T>(z, y, x, y); }
	Vec4<T> zyxz() const { return Vec4<T>(z, y, x, z); }
	Vec4<T> zyxw() const { return Vec4<T>(z, y, x, w); }
	Vec4<T> zyyx() const { return Vec4<T>(z, y, y, x); }
	Vec4<T> zyyy() const { return Vec4<T>(z, y, y, y); }
	Vec4<T> zyyz() const { return Vec4<T>(z, y, y, z); }
	Vec4<T> zyyw() const { return Vec4<T>(z, y, y, w); }
	Vec4<T> zyzx() const { return Vec4<T>(z, y, z, x); }
	Vec4<T> zyzy() const { return Vec4<T>(z, y, z, y); }
	Vec4<T> zyzz() const { return Vec4<T>(z, y, z, z); }
	Vec4<T> zyzw() const { return Vec4<T>(z, y, z, w); }
	Vec4<T> zywx() const { return Vec4<T>(z, y, w, x); }
	Vec4<T> zywy() const { return Vec4<T>(z, y, w, y); }
	Vec4<T> zywz() const { return Vec4<T>(z, y, w, z); }
	Vec4<T> zyww() const { return Vec4<T>(z, y, w, w); }
	Vec4<T> zzxx() const { return Vec4<T>(z, z, x, x); }
	Vec4<T> zzxy() const { return Vec4<T>(z, z, x, y); }
	Vec4<T> zzxz() const { return Vec4<T>(z, z, x, z); }
	Vec4<T> zzxw() const { return Vec4<T>(z, z, x, w); }
	Vec4<T> zzyx() const { return Vec4<T>(z, z, y, x); }
	Vec4<T> zzyy() const { return Vec4<T>(z, z, y, y); }
	Vec4<T> zzyz() const { return Vec4<T>(z, z, y, z); }
	Vec4<T> zzyw() const { return Vec4<T>(z, z, y, w); }
	Vec4<T> zzzx() const { return Vec4<T>(z, z, z, x); }
	Vec4<T> zzzy() const { return Vec4<T>(z, z, z, y); }
	Vec4<T> zzzz() const { return Vec4<T>(z, z, z, z); }
	Vec4<T> zzzw() const { return Vec4<T>(z, z, z, w); }
	Vec4<T> zzwx() const { return Vec4<T>(z, z, w, x); }
	Vec4<T> zzwy() const { return Vec4<T>(z, z, w, y); }
	Vec4<T> zzwz() const { return Vec4<T>(z, z, w, z); }
	Vec4<T> zzww() const { return Vec4<T>(z, z, w, w); }
	Vec4<T> zwxx() const { return Vec4<T>(z, w, x, x); }
	Vec4<T> zwxy() const { return Vec4<T>(z, w, x, y); }
	Vec4<T> zwxz() const { return Vec4<T>(z, w, x, z); }
	Vec4<T> zwxw() const { return Vec4<T>(z, w, x, w); }
	Vec4<T> zwyx() const { return Vec4<T>(z, w, y, x); }
	Vec4<T> zwyy() const { return Vec4<T>(z, w, y, y); }
	Vec4<T> zwyz() const { return Vec4<T>(z, w, y, z); }
	Vec4<T> zwyw() const { return Vec4<T>(z, w, y, w); }
	Vec4<T> zwzx() const { return Vec4<T>(z, w, z, x); }
	Vec4<T> zwzy() const { return Vec4<T>(z, w, z, y); }
	Vec4<T> zwzz() const { return Vec4<T>(z, w, z, z); }
	Vec4<T> zwzw() const { return Vec4<T>(z, w, z, w); }
	Vec4<T> zwwx() const { return Vec4<T>(z, w, w, x); }
	Vec4<T> zwwy() const { return Vec4<T>(z, w, w, y); }
	Vec4<T> zwwz() const { return Vec4<T>(z, w, w, z); }
	Vec4<T> zwww() const { return Vec4<T>(z, w, w, w); }
	Vec4<T> wxxx() const { return Vec4<T>(w, x, x, x); }
	Vec4<T> wxxy() const { return Vec4<T>(w, x, x, y); }
	Vec4<T> wxxz() const { return Vec4<T>(w, x, x, z); }
	Vec4<T> wxxw() const { return Vec4<T>(w, x, x, w); }
	Vec4<T> wxyx() const { return Vec4<T>(w, x, y, x); }
	Vec4<T> wxyy() const { return Vec4<T>(w, x, y, y); }
	Vec4<T> wxyz() const { return Vec4<T>(w, x, y, z); }
	Vec4<T> wxyw() const { return Vec4<T>(w, x, y, w); }
	Vec4<T> wxzx() const { return Vec4<T>(w, x, z, x); }
	Vec4<T> wxzy() const { return Vec4<T>(w, x, z, y); }
	Vec4<T> wxzz() const { return Vec4<T>(w, x, z, z); }
	Vec4<T> wxzw() const { return Vec4<T>(w, x, z, w); }
	Vec4<T> wxwx() const { return Vec4<T>(w, x, w, x); }
	Vec4<T> wxwy() const { return Vec4<T>(w, x, w, y); }
	Vec4<T> wxwz() const { return Vec4<T>(w, x, w, z); }
	Vec4<T> wxww() const { return Vec4<T>(w, x, w, w); }
	Vec4<T> wyxx() const { return Vec4<T>(w, y, x, x); }
	Vec4<T> wyxy() const { return Vec4<T>(w, y, x, y); }
	Vec4<T> wyxz() const { return Vec4<T>(w, y, x, z); }
	Vec4<T> wyxw() const { return Vec4<T>(w, y, x, w); }
	Vec4<T> wyyx() const { return Vec4<T>(w, y, y, x); }
	Vec4<T> wyyy() const { return Vec4<T>(w, y, y, y); }
	Vec4<T> wyyz() const { return Vec4<T>(w, y, y, z); }
	Vec4<T> wyyw() const { return Vec4<T>(w, y, y, w); }
	Vec4<T> wyzx() const { return Vec4<T>(w, y, z, x); }
	Vec4<T> wyzy() const { return Vec4<T>(w, y, z, y); }
	Vec4<T> wyzz() const { return Vec4<T>(w, y, z, z); }
	Vec4<T> wyzw() const { return Vec4<T>(w, y, z, w); }
	Vec4<T> wywx() const { return Vec4<T>(w, y, w, x); }
	Vec4<T> wywy() const { return Vec4<T>(w, y, w, y); }
	Vec4<T> wywz() const { return Vec4<T>(w, y, w, z); }
	Vec4<T> wyww() const { return Vec4<T>(w, y, w, w); }
	Vec4<T> wzxx() const { return Vec4<T>(w, z, x, x); }
	Vec4<T> wzxy() const { return Vec4<T>(w, z, x, y); }
	Vec4<T> wzxz() const { return Vec4<T>(w, z, x, z); }
	Vec4<T> wzxw() const { return Vec4<T>(w, z, x, w); }
	Vec4<T> wzyx() const { return Vec4<T>(w, z, y, x); }
	Vec4<T> wzyy() const { return Vec4<T>(w, z, y, y); }
	Vec4<T> wzyz() const { return Vec4<T>(w, z, y, z); }
	Vec4<T> wzyw() const { return Vec4<T>(w, z, y, w); }
	Vec4<T> wzzx() const { return Vec4<T>(w, z, z, x); }
	Vec4<T> wzzy() const { return Vec4<T>(w, z, z, y); }
	Vec4<T> wzzz() const { return Vec4<T>(w, z, z, z); }
	Vec4<T> wzzw() const { return Vec4<T>(w, z, z, w); }
	Vec4<T> wzwx() const { return Vec4<T>(w, z, w, x); }
	Vec4<T> wzwy() const { return Vec4<T>(w, z, w, y); }
	Vec4<T> wzwz() const { return Vec4<T>(w, z, w, z); }
	Vec4<T> wzww() const { return Vec4<T>(w, z, w, w); }
	Vec4<T> wwxx() const { return Vec4<T>(w, w, x, x); }
	Vec4<T> wwxy() const { return Vec4<T>(w, w, x, y); }
	Vec4<T> wwxz() const { return Vec4<T>(w, w, x, z); }
	Vec4<T> wwxw() const { return Vec4<T>(w, w, x, w); }
	Vec4<T> wwyx() const { return Vec4<T>(w, w, y, x); }
	Vec4<T> wwyy() const { return Vec4<T>(w, w, y, y); }
	Vec4<T> wwyz() const { return Vec4<T>(w, w, y, z); }
	Vec4<T> wwyw() const { return Vec4<T>(w, w, y, w); }
	Vec4<T> wwzx() const { return Vec4<T>(w, w, z, x); }
	Vec4<T> wwzy() const { return Vec4<T>(w, w, z, y); }
	Vec4<T> wwzz() const { return Vec4<T>(w, w, z, z); }
	Vec4<T> wwzw() const { return Vec4<T>(w, w, z, w); }
	Vec4<T> wwwx() const { return Vec4<T>(w, w, w, x); }
	Vec4<T> wwwy() const { return Vec4<T>(w, w, w, y); }
	Vec4<T> wwwz() const { return Vec4<T>(w, w, w, z); }
	Vec4<T> wwww() const { return Vec4<T>(w, w, w, w); }

	friend std::ostream& operator<<(std::ostream& lhs, const Vec4<T>& rhs)
	{
		lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
		return lhs;
	}

	static Vec4<T> xAxis() { return Vec4<T>(1, 0, 0, 0); }
	static Vec4<T> yAxis() { return Vec4<T>(0, 1, 0, 0); }
	static Vec4<T> zAxis() { return Vec4<T>(0, 0, 1, 0); }
	static Vec4<T> wAxis() { return Vec4<T>(0, 0, 0, 1); }

	
};

typedef Vec2<int>		Vec2i;
typedef Vec2<float>		Vec2f;
typedef Vec2<double>	Vec2d;
typedef Vec3<int>		Vec3i;
typedef Vec3<float>		Vec3f;
typedef Vec3<double>	Vec3d;
typedef Vec4<int>		Vec4i;
typedef Vec4<float>		Vec4f;
typedef Vec4<double>	Vec4d;






static bool collisionPlateToPlate(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2, const Vec2f& size2) {
	if (pos1.x < pos2.x + size2.x) {
		if (pos1.x + size1.x > pos2.x) {
			if (pos1.y < pos2.y + size2.y) {
				if (pos1.y + size1.y > pos2.y) {
					return true;
				}
			}
		}
	}
	return false;
}

static bool collisionPlateToPoint(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2) {
	if (pos1.x < pos2.x) {
		if (pos1.x + size1.x > pos2.x) {
			if (pos1.y < pos2.y) {
				if (pos1.y + size1.y > pos2.y) {
					return true;
				}
			}
		}
	}
	return false;
}

}
