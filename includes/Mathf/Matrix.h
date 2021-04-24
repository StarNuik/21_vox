#include "Quaternion.h"
#include <cstdio>
#include <cmath>
#include <cstring>

namespace mathf
{

	struct mat4x4
	{
	float data[4][4];

		explicit mat4x4()
		{
			::memset(data, 0x0, sizeof(data));
		}

		mat4x4(const glm::mat4& mat)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					data[i][j] = mat[i][j];
				}
			}
		}

		mat4x4& operator=(const mat4x4& mat)
		{
			::memcpy(this->data, &mat.data, sizeof(data));
			return *this;
		}

		mat4x4 operator*(const mat4x4& mat) const
		{
			/*
(*this)[i * w + k] * rhs[k * rhs.w + j];
			*/
			mat4x4 res;
			for (unsigned int i = 0; i < 4; i++)
				for (unsigned int j = 0; j < 4; j++)
				{
					float rs = 0.f;
					for (unsigned int k = 0; k < 4; k++)
						rs += mat[i][k] * data[k][j]; 
					res[i][j] = rs;
				}

			return res;
		}

		vec4 operator*(const vec4& vec) const
		{
			vec4 res;

			res.x = data[0][0] * vec.x + data[1][0] * vec.y + data[2][0] * vec.z + data[3][0] * vec.w;
			res.y = data[0][1] * vec.x + data[1][1] * vec.y + data[2][1] * vec.z + data[3][1] * vec.w;
			res.z = data[0][2] * vec.x + data[1][2] * vec.y + data[2][2] * vec.z + data[3][2] * vec.w;
			res.w = data[0][3] * vec.x + data[1][3] * vec.y + data[2][3] * vec.z + data[3][3] * vec.w;
			
			return res;
		}

		const float* operator[](unsigned int index) const
		{
			return data[index];
		}

		float* operator[](unsigned int index)
		{
			return data[index];
		}

		inline
		static mat4x4  identity()
		{
			mat4x4 res = mat4x4();

			res[0][0] = 1.f;
			res[1][1] = 1.f;
			res[2][2] = 1.f;
			res[3][3] = 1.f;

			return res;
		}

		inline
		static mat4x4 rotate_x(mat4x4& mat, float angle)
		{
			float		my_cos;
			float		my_sin;
			mat4x4	    new_mat;

			my_cos = cosf(angle);
			my_sin = sinf(angle);
			new_mat = identity();
			new_mat[1][1] = my_cos;
			new_mat[1][2] = my_sin;
			new_mat[2][1] = -my_sin;
			new_mat[2][2] = my_cos;
			
			new_mat = mat * new_mat;
			return new_mat;
		}

		inline
		static mat4x4 rotate_y(mat4x4& mat, float angle)
		{
			float		my_cos;
			float		my_sin;
			mat4x4	    new_mat;

			my_cos = cosf(angle);
			my_sin = sinf(angle);
			new_mat = identity();
			new_mat[0][0] = my_cos;
			new_mat[0][2] = -my_sin;
			new_mat[2][0] = my_sin;
			new_mat[2][2] = my_cos;

			new_mat = mat * new_mat;
			return new_mat;
		}

		inline
		static mat4x4 rotate_z(mat4x4& mat, float angle)
		{
			float		my_cos;
			float		my_sin;
			mat4x4	    new_mat;

			my_cos = cosf(angle);
			my_sin = sinf(angle);
			new_mat = identity();
			new_mat[0][0] = my_cos;
			new_mat[0][1] = -my_sin;
			new_mat[1][0] = my_sin;
			new_mat[1][1] = my_cos;

			new_mat = mat * new_mat;
			return new_mat;
		}

		inline
		static mat4x4 projection(float fov, float aspect, float near, float far)
		{
			mat4x4      new_mat;
			float		v1;

			v1 = tanf(fov / 2.f);
			new_mat[0][0] = 1.f / (aspect * v1);
			new_mat[1][1] = 1.f / v1;
			new_mat[2][2] = -(far + near) / (far - near);
			new_mat[2][3] = -1.f;
			new_mat[3][2] = -(2.f * near * far) / (far - near);

			return new_mat;
		}

		inline
		static mat4x4 translate(mat4x4& mat, vec3& dir)
		{
			mat4x4 new_mat;
			
			new_mat = identity();
			new_mat[3][0] = dir.x;
			new_mat[3][1] = dir.y;
			new_mat[3][2] = dir.z;

			new_mat = mat * new_mat;
			return new_mat;
		}

		// static mat4x4	scale(mat4x4 mat, vec3 scale)
		// {
		// 	mat4x4 new_mat;

		// 	new_mat = identity();
		// 	new_mat[0][0] = scale.x;
		// 	new_mat[1][1] = scale.y;
		// 	new_mat[2][2] = scale.z;

		// 	new_mat = mat * new_mat;
		// 	return new_mat;
		// }

		inline
		static mat4x4	scale(const mat4x4& mat, const vec3& scale)
		{
			mat4x4 new_mat;

			new_mat = identity();
			new_mat[0][0] = scale.x;
			new_mat[1][1] = scale.y;
			new_mat[2][2] = scale.z;

			new_mat = mat * new_mat;
			return new_mat;
		}

		inline
		static mat4x4	look_at(const vec3 from, const vec3 to, const vec3 cam_up)
		{
			mat4x4	    cam_to_world;
			// vec3 const f(normalize(to - from));
			// vec3 const s(normalize(cross(f, up)));
			// vec3 const u(cross(s, f));

			// cam_to_world[0][0] = s.x;
			// cam_to_world[1][0] = s.y;
			// cam_to_world[2][0] = s.z;
			// cam_to_world[0][1] = u.x;
			// cam_to_world[1][1] = u.y;
			// cam_to_world[2][1] = u.z;
			// cam_to_world[0][2] =-f.x;
			// cam_to_world[1][2] =-f.y;
			// cam_to_world[2][2] =-f.z;
			// cam_to_world[3][0] =-dot(s, eye);
			// cam_to_world[3][1] =-dot(u, eye);
			// cam_to_world[3][2] = dot(f, eye);

			vec3		forward;
			vec3		right;
			vec3		up;

			cam_to_world = identity();

			forward = mathf::vec3::normalize(to - from);
			right = mathf::vec3::normalize(mathf::vec3::cross(forward, cam_up));
			up = mathf::vec3::cross(right, forward);
			// forward = (to - from).normalize();
			// right = forward.cross(cam_up).normalize();
			// up = right.cross(forward);

			cam_to_world[0][0] = right.x;
			cam_to_world[1][0] = right.y;
			cam_to_world[2][0] = right.z;
			cam_to_world[0][1] = up.x;
			cam_to_world[1][1] = up.y;
			cam_to_world[2][1] = up.z;
			cam_to_world[0][2] = -forward.x;
			cam_to_world[1][2] = -forward.y;
			cam_to_world[2][2] = -forward.z;
			cam_to_world[3][0] = -right.dot(from);
			cam_to_world[3][1] = -up.dot(from);
			cam_to_world[3][2] = forward.dot(from);

			return cam_to_world;
		}

		inline
		static mat4x4 ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
		{
			mat4x4 res = identity();
			
			res[0][0] = 2.f / (right - left);
			res[1][1] = 2.f / (top - bottom);
			res[2][2] = - 2.f / (zFar - zNear);
			res[3][0] = - (right + left) / (right - left);
			res[3][1] = - (top + bottom) / (top - bottom);
			res[3][2] = - (zFar + zNear) / (zFar - zNear);
			
			return res;
		}


		inline
		static mat4x4 cast(const quat& q)
		{
			mat4x4 new_mat = identity();

			float qxx = q.x * q.x;
			float qyy = q.y * q.y;
			float qzz = q.z * q.z;
			float qxz = q.x * q.z;
			float qxy = q.x * q.y;
			float qyz = q.y * q.z;
			float qwx = q.w * q.x;
			float qwy = q.w * q.y;
			float qwz = q.w * q.z;

			new_mat[0][0] = 1.f - 2.f * (qyy +  qzz);
			new_mat[0][1] = 2.f * (qxy + qwz);
			new_mat[0][2] = 2.f * (qxz - qwy);

			new_mat[1][0] = 2.f * (qxy - qwz);
			new_mat[1][1] = 1.f - 2.f * (qxx +  qzz);
			new_mat[1][2] = 2.f * (qyz + qwx);

			new_mat[2][0] = 2.f * (qxz + qwy);
			new_mat[2][1] = 2.f * (qyz - qwx);
			new_mat[2][2] = 1.f - 2.f * (qxx +  qyy);

			return new_mat;
		}

		inline
		static void    decompose(mat4x4& mat, vec3& scale, quat& rotation, vec3& position)
		{
			vec3 row[3];

			for(unsigned int i = 0; i < 3; ++i)
			{
				row[i].x = mat[i][0];
				row[i].y = mat[i][1];
				row[i].z = mat[i][2];
			}

			scale.x = row[0].length();
			scale.y = row[1].length();
			scale.z = row[2].length();

			float root, trace = row[0].x + row[1].y + row[2].z;

			root = sqrt(trace + 1.f); 
			rotation.w = 0.5f * root;
			root = 0.5f / root;
			int i, j, k = 0;

			if (trace > 0.f)
			{
				rotation.x = root * (row[1].z - row[2].y);
				rotation.y = root * (row[2].x - row[0].z);
				rotation.z = root * (row[0].y - row[1].x);
			}
			else
			{
				static int next[3] = {1, 2, 0};
				i = 0;
				if(row[1].y > row[0].x) 
				{
					i = 1;
				}
				if(row[2].z > row[i][i])
				{
					i = 2;
				}
				j = next[i];
				k = next[j];

				root = sqrt(row[i][i] - row[j][j] - row[k][k] + 1.f);

				rotation[i] = 0.5f * root;
				root = 0.5f / root;
				rotation[j] = root * (row[i][j] + row[j][i]);
				rotation[k] = root * (row[i][k] + row[k][i]);
				rotation.w = root * (row[j][k] - row[k][j]);
			}

			position.x = mat[3][0];
			position.y = mat[3][1];
			position.z = mat[3][2];
		}


		glm::mat4x4 to_glm()
		{
			glm::mat4 mat;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					mat[i][j] = data[i][j];
				}
			}

			return mat;
		}
	};

}
