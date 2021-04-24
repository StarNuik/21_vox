#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace mathf
{
	inline
	vec3 operator*(vec3 v, quat q)
	{
		return quat::inverse(q) * v;
	}

	inline
	float radians(float degrees) noexcept
	{
		return degrees * 0.01745329251994f;
	}

	inline
	float degrees(float radians) noexcept
	{
		return radians * 57.2957795130823f;
	}

	inline
	float min(float left, float right) noexcept
	{
		return left < right ? left : right;
	}

	inline
	float max(float left, float right) noexcept
	{
		return left > right ? left : right; 
	}
    
}