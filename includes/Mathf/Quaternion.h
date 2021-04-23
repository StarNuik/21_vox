#pragma once

#include "Vector.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace mathf
{
	struct quat
	{
		float x;
		float y;
		float z;
		float w;

		quat()
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 0.f;
		}

		quat(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		explicit quat(const vec3& vec)
		{
			float cx = cosf(vec.x * 0.5f);
			float cy = cosf(vec.y * 0.5f);
			float cz = cosf(vec.z * 0.5f);
			float sx = sinf(vec.x * 0.5f);
			float sy = sinf(vec.y * 0.5f);
			float sz = sinf(vec.z * 0.5f);

			this->w = cx * cy * cz + sx * sy * sz;
			this->x = sx * cy * cz - cx * sy * sz;
			this->y = cx * sy * cz + sx * cy * sz;
			this->z = cx * cy * sz - sx * sy * cz;
		}

		quat(const glm::quat& qw)
		{
			x = qw.x;
			y = qw.y;
			z = qw.z;
			w = qw.w;
		}

		quat& operator=(const quat& q)
		{
			this->x = q.x;
			this->y = q.y;
			this->z = q.z;
			this->w = q.w;

			return *this;
		}

		quat& operator+=(const quat& q)
		{
			this->x += q.x;
			this->y += q.y;
			this->z += q.z;
			this->w += q.w;

			return *this;
		}

		quat& operator-=(const quat& q)
		{
			this->x -= q.x;
			this->y -= q.y;
			this->z -= q.z;
			this->w -= q.w;

			return *this;
		}

		quat& operator*=(const quat& q)
		{
			quat p = *this;
		
			this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
			this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
			this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
			this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;

			return *this;
		}


		quat operator-(const quat& q)
		{
			quat res;

			res.x = this->x - q.x;
			res.y = this->y - q.y;
			res.z = this->z - q.z;
			res.w = this->w - q.w;

			*this = res;
			return res;
		}

		const quat operator+(const quat& q) const
		{
			quat res = *this;

			res += q;

			return res;
		}


		quat operator*(const quat& q) const
		{
			quat res = *this;

			res *= q;

			return res;
		}

		mathf::vec3 operator*(const mathf::vec3& v) const
		{
			mathf::vec3 quatVec = (mathf::vec3){this->x, this->y, this->z};
			mathf::vec3 uv = quatVec.cross(v);
			mathf::vec3 uuv = quatVec.cross(uv);
			// mathf::vec3 uv = cross(quatVec, v);
			// mathf::vec3 uuv = cross(quatVec, uv);
			
			return v + ((uv * this->w) + uuv) * 2.f;
		}

		quat operator*(const float& value) const
		{
			return (quat){this->w * value, this->x * value, this->y * value, this->z * value};
		}

		const float operator[](int i) const
		{
			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else if (i == 2)
				return z;
			else
				return w;
		}

		float operator[](int i)
		{
			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else if (i == 2)
				return z;
			else
				return w;
		}

		inline
		static quat		identity()
		{
			return ((quat){0.f, 0.f, 0.f, 1.f});
		}

		inline
		static float	legth(quat& q)
		{
			return (sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w));
		}

		inline
		static quat	normalize(quat& q)
		{
			quat	res;
			float	length;

			length = legth(q);
			res.x = q.x / length;
			res.y = q.y / length;
			res.z = q.z / length;
			res.w = q.w / length;
			return (res);
		}

		inline
		static quat inverse(quat& q)
		{
			quat res;

			res.x = -q.x;
			res.y = -q.y;
			res.z = -q.z;
			res.w = q.w;

			res = normalize(res);
			return res;
		}

		inline
		static float clamp(float value, float min, float max)
		{
			if (value < min)
			{
				value = min;
			}
			else if (value > max)
			{
				value = max;
			}

			return value;
		}

		inline
		static mathf::vec3 euler_from_quat(quat& q)
		{
			float roll = atan2f(2.f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
			float pitch;
			float y = 2.f * (q.y * q.z + q.w * q.x);
			float x = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
			pitch = atan2f(y, x);
			float yaw = asin(clamp(-2.f * (q.x * q.z - q.w * q.y), -1.f, 1.f));

			return mathf::vec3(roll, pitch, yaw);
		}

		inline
		static quat create_from_axis_angle(const mathf::vec3& axis, const float& angle)
		{
			float halfAngle = angle * 0.5f;
			float s = sinf(halfAngle);
			quat q = quat(axis.x * s, axis.y * s, axis.z * s, cosf(halfAngle));
			return q;
		}

		inline
		static quat look_at(const mathf::vec3& direction, const mathf::vec3& up)
		{
			float mdot = mathf::vec3(0.f, 0.f, 1.f).dot(direction);

			if (abs(mdot - 1.f) < 0.000001f)
			{
				return quat(up.x, up.y, up.z, M_PI);
			}
			else if (abs(mdot - 1.f) < 0.000001f)
			{
				return identity();
			}

			float rotAngle = (float)acosf(mdot);
			mathf::vec3 rotAxis = (mathf::vec3::cross(mathf::vec3(0.f, 0.f, 1.f), direction)).normalize();
			
			return create_from_axis_angle(rotAxis, rotAngle);
		}

		inline
		glm::quat to_glm()
		{
			return glm::quat(this->w, this->x, this->y, this->z);
		}
	};
}