#pragma once
#include "Quaternion.h"
#include "Vector.h"


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
    
}