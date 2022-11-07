#pragma once
#pragma warning( disable : 26451 )
#include <cmath>
#include <limits>
#include <algorithm>

inline float bits_to_float(std::uint32_t i)
{
	union convertor_t { float f; unsigned long ul;
	} tmp{};

	tmp.ul = i;
	return tmp.f;
}

constexpr double M_PI = 3.14159265358979323846;
[[maybe_unused]] constexpr float M_RADPI = 57.295779513082f;
constexpr float M_PI_F = static_cast<float>(M_PI);

constexpr float RAD2DEG(const float x)
{
	return (float)(x) * (float)(180.f / M_PI_F);
}

constexpr float DEG2RAD(const float x)
{
	return (float)(x) * (float)(M_PI_F / 180.f);
}

constexpr std::uint32_t FLOAT32_NAN_BITS = 0x7FC00000;
const float FLOAT32_NAN = bits_to_float(FLOAT32_NAN_BITS);
#define VEC_T_NAN FLOAT32_NAN
#define ASSERT( _exp ) ( (void ) 0 )

class vec3_t
{
public:
	float x, y, z;

	vec3_t()
	{
		x = 0; y = 0; z = 0;
	}

	vec3_t(float _x, float _y, float _z)
	{
		x = _z; y = _y; z = _z;
	}

	vec3_t& operator+=(const vec3_t& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	vec3_t& operator-=(const vec3_t& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	vec3_t& operator*=(float v)
	{
		x *= v; y *= v; z *= v; return *this;
	}

	vec3_t operator+(const vec3_t& v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	vec3_t operator-(const vec3_t& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	vec3_t operator*(float fl) const
	{
		return {x * fl, y * fl, z * fl};
	}

	vec3_t operator*(const vec3_t& v) const
	{
		return {x * v.x, y * v.y, z * v.z};
	}

	vec3_t& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	auto operator/(float other) const
	{
		vec3_t vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	auto operator==( vec3_t o ) const
	{
		return x == o.x && y == o.y && z == o.z;
	}

	auto operator!=( vec3_t o ) const
	{
		return !( vec3_t( x, y, z ) == o );
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	[[nodiscard]] inline float length_2d() const
	{
		return std::sqrt((x * x) + (y * y));
	}

	static void crossproduct(vec3_t v1, vec3_t v2, vec3_t cross_p)
	{
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}

	[[nodiscard]] vec3_t cross(const vec3_t& other) const
	{
		vec3_t res;
		crossproduct(*this, other, res);
		return res;
	}

	void init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
	}

	void clamp()
	{
		x = std::clamp( x, -89.0f, 89.0f );
		y = std::clamp( std::remainder( y, 360.0f ), -180.0f, 180.0f );
		z = std::clamp( z, -50.0f, 50.0f );
	}

	vec3_t clamped()
	{
		vec3_t clamped = *this;
		clamped.clamp( );
		return clamped;
	}

	[[maybe_unused]] [[nodiscard]] float distance_to(const vec3_t& other) const
	{
		vec3_t delta;
		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length( );
	}

	void normalize()
	{
		x = std::isfinite( x ) ? std::remainderf( x, 360.0f ) : 0.0f;
		y = std::isfinite( y ) ? std::remainderf( y, 360.0f ) : 0.0f;
		z = 0.0f;
	}

	vec3_t normalized()
	{
		vec3_t vec( *this );
		vec.normalize( );

		return vec;
	}

	[[maybe_unused]] float length_normal( )
	{
		float len = length( );

		( *this ) /= ( length( ) + std::numeric_limits< float >::epsilon( ) );

		return len;
	}

	[[nodiscard]] float length() const
	{
		float root = 0.0f, sqsr = this->length_sqr( );

		__asm        sqrtss xmm0, sqsr
		__asm        movss root, xmm0

		return root;
	}

	[[nodiscard]] float length_sqr() const
	{
		auto sqr = []( float n ) {
			return static_cast<float>( n * n );
		};

		return ( sqr( x ) + sqr( y ) + sqr( z ) );
	}

	[[nodiscard]] float length_2d_sqr() const
	{
		return ( x * x + y * y );
	}

	[[nodiscard]] float dot(const vec3_t other) const
	{
		return ( x * other.x + y * other.y + z * other.z );
	}

	float dot(const float* other)
	{
		const vec3_t& a = *this;

		return( a.x * other[0] + a.y * other[1] + a.z * other[2] );
	}

};

inline vec3_t operator*(float lhs, const vec3_t& rhs)
{
	return {rhs.x * lhs, rhs.x * lhs, rhs.x * lhs};
}

struct matrix_t
{
	matrix_t() { }
	matrix_t(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23)
	{
		mat_val[0][0] = m00;	mat_val[0][1] = m01; mat_val[0][2] = m02; mat_val[0][3] = m03;
		mat_val[1][0] = m10;	mat_val[1][1] = m11; mat_val[1][2] = m12; mat_val[1][3] = m13;
		mat_val[2][0] = m20;	mat_val[2][1] = m21; mat_val[2][2] = m22; mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin)
	{
		mat_val[0][0] = x.x; mat_val[0][1] = y.x; mat_val[0][2] = z.x; mat_val[0][3] = origin.x;
		mat_val[1][0] = x.y; mat_val[1][1] = y.y; mat_val[1][2] = z.y; mat_val[1][3] = origin.y;
		mat_val[2][0] = x.z; mat_val[2][1] = y.z; mat_val[2][2] = z.z; mat_val[2][3] = origin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	[[maybe_unused]] matrix_t(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin)
	{
		init(x, y, z, origin);
	}

	inline void set_origin(vec3_t const& p)
	{
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}

	inline void invalidate()
	{
		for (auto & i : mat_val)
			for (int j = 0; j < 4; j++)
				i[j] = VEC_T_NAN;
	}

	float* operator[](int i) { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	const float* operator[](int i) const { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	float* base() { return &mat_val[0][0]; }
	[[nodiscard]] const float* base() const { return &mat_val[0][0]; }

	float mat_val[3][4];
};

class vec2_t
{
public:
	float x, y;

	vec2_t()
	{
		x = 0; y = 0;
	};

	vec2_t(float _x, float _y)
	{
		x = _x; y = _y;
	};

	explicit vec2_t(vec3_t vec)
	{
		x = vec.x; y = vec.y;
	}

	inline vec2_t operator*(const float n) const
	{
		return {x * n, y * n};
	}

	inline vec2_t operator+(const vec2_t& v) const
	{
		return {x + v.x, y + v.y};
	}

	inline vec2_t operator-(const vec2_t & v) const
	{
		return {x - v.x, y - v.y};
	}

	inline void operator+=(const vec2_t & v)
	{
		x += v.x;
		y += v.y;
	}

	inline void operator-=(const vec2_t & v)
	{
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const vec2_t & v) const
	{
		return (v.x == x && v.y == y);
	}

	bool operator!=(const vec2_t & v) const
	{
		return (v.x != x || v.y != y);
	}

	[[nodiscard]] inline float length() const
	{
		return sqrt((x * x) + (y * y));
	}
};