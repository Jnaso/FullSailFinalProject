#pragma once
//#define _XM_NO_INTRINSICS_

#include <vector>
#include <array>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <dinput.h>
#include <time.h>
#include <stdlib.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

const int NUM_LIGHTS = 2;

enum KEYSTATE { UP, DOWN };

struct float2
{
	float x;
	float y;

	inline float& operator[](int i) { return (&x)[i]; }
	inline float operator[](int i)const { return (&x)[i]; }

	inline float* data() { return &x; }
	inline const float* data()const { return &x; }
	inline constexpr size_t size()const { return 2; }
};

struct float3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float2 xy;
	};


	inline float& operator[](int i) { return (&x)[i]; }
	inline float operator[](int i)const { return (&x)[i]; }

	inline float* data() { return &x; }
	inline const float* data()const { return &x; }
	inline constexpr size_t size()const { return 3; }

	inline float3& normalize() {
		// compute distance
		float dist = sqrt(x*x + y * y + z * z);
		if (dist == 0)
		{
			x = y = z = 0;
			return *this;
		}
		// divide components by distance
		x /= dist;
		y /= dist;
		z /= dist;
		return *this;
	}

	inline float magnitude()
	{
		return sqrt((this->x*this->x) + (this->y*this->y) + (this->z*this->z));
	}

	inline float Squaremagnitude()const
	{
		float mag = sqrt((this->x*this->x) + (this->y*this->y) + (this->z*this->z));
		return mag;
	}

	inline void operator*=(float& value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
	}

	inline float3& operator*(const float& value) const
	{
		float3 NewVal = { this->x * value, this->y * value, this->z * value };
		return NewVal;
	}

	inline void operator+=(const float3& value)
	{
		this->x += value.x;
		this->y += value.y;
		this->z += value.z;
	}

	inline float3& operator+(const float3& value) const
	{
		float3 newVal = { this->x + value.x, this->y + value.y, this->z + value.z };
		return newVal;
	}

	inline void operator-=(const float3& value)
	{
		this->x -= value.x;
		this->y -= value.y;
		this->z -= value.z;
	}

	inline float3& operator-(const float3& value) const
	{
		float3 newVal = { this->x - value.x, this->y - value.y, this->z - value.z };
		return newVal;
	}

	inline void addScaledVec(const float3& vec, const float& scale)
	{
		this->x += vec.x * scale;
		this->y += vec.y * scale;
		this->z += vec.z * scale;
	}

	inline float3 componentProduct(const float3& vec) const
	{
		float3 newVec = { this->x * vec.x, this->y * vec.y, this->z * vec.z };
		return newVec;
	}

	inline void updateComponentProduct(const float3& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
	}

	inline float scalarProduct(const float3& vec)const
	{
		return this->x * vec.x + this->y * vec.y + this->z * vec.z;
	}

	inline float& operator*(const float3& vec)const
	{
		float value = this->x * vec.x + this->y * vec.y + this->z * vec.z;
		return value;
	}

	inline float3 vectorProduct(const float3& vec) const
	{
		float3 newVec = { this->y*vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x*vec.y - this->y*vec.x };
		return newVec;
	}

	inline void operator%=(const float3& vec)
	{
		*this = this->vectorProduct(vec);
	}

	inline float3& operator%(const float3& vec) const
	{
		float3 newVec = { this->y*vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x*vec.y - this->y*vec.x };
		return newVec;
	}

	void makeOrthonormalBasis(float3& vecA, float3& vecB, float3& vecC)
	{
		vecA.normalize();
		vecC = vecA % vecB;
		if (vecC.magnitude() == 0.0f) return;
		vecC.normalize();
		vecB = vecC % vecA;
	}
};

struct float4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		float3 xyz;

		struct
		{
			float2 xy;
			float2 zw;
		};
	};

	inline float& operator[](int i) { return (&x)[i]; }
	inline float operator[](int i)const { return (&x)[i]; }

	inline float* data() { return &x; }
	inline const float* data()const { return &x; }
	inline constexpr size_t size()const { return 4; }
};

inline float4 Add(float4 a, float4 b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; };

struct alignas(8) float2_a : float2 {};

struct alignas(16) float3_a : float3 {};

struct alignas(16) float4_a : float4 {};

using float4x4 = std::array< float4, 4 >;
using float4x4_a = std::array< float4_a, 4 >;

struct Vertex
{
	float3 pos, norm;
	float2 texCords;
	float3 Tangent;
	int joints[4];
	float4 weights;
};

struct keyframe
{
	double time; std::vector<float4x4> joints;
};

struct anim_clip
{
	double duration;
	std::vector<keyframe> frames;
	std::vector<int32_t> parent_indicies;
	keyframe bindPose;
};

using file_path_t = std::array<char, 260>;

struct material_t
{
	enum component { EMISSIVE = 0, DIFFUSE, NORMAL, SPECULAR, SHININESS, COUNT };

	struct component_t
	{
		float value[3]{ 0.0f, 0.0f, 0.0f };
		float factor = 0.0f;
		int64_t input = -1;
	};

	component_t& operator[](int i) { return components[i]; };
	const component_t& operator[](int i)const { return components[i]; };

private:
	component_t components[COUNT];
};

inline float4x4 XMMatrixToFloat4x4(XMMATRIX input)
{
	float4x4 output;
	for (int i = 0; i < 4; i++)
	{
		output[i][0] = input.r[i].m128_f32[0];
		output[i][1] = input.r[i].m128_f32[1];
		output[i][2] = input.r[i].m128_f32[2];
		output[i][3] = input.r[i].m128_f32[3];
	}
	return output;
}

inline XMMATRIX Float4x4ToXMMatrix(float4x4 input)
{
	XMMATRIX output;
	for (int i = 0; i < 4; i++)
	{
		output.r[i].m128_f32[0] = input[i][0];
		output.r[i].m128_f32[1] = input[i][1];
		output.r[i].m128_f32[2] = input[i][2];
		output.r[i].m128_f32[3] = input[i][3];
	}
	return output;
}

inline float4 XMVectortofloat4(XMVECTOR input)
{
	float4 output;
	output.x = input.m128_f32[0];
	output.y = input.m128_f32[1];
	output.z = input.m128_f32[2];
	output.w = input.m128_f32[3];
	return output;
}

inline XMVECTOR float4toXMVector(float4 input)
{
	XMVECTOR output;
	output.m128_f32[0] = input.x;
	output.m128_f32[1] = input.y;
	output.m128_f32[2] = input.z;
	output.m128_f32[3] = input.w;
	return output;
}

inline XMVECTOR float3toXMVector(float3 input)
{
	XMVECTOR output;
	output.m128_f32[0] = input.x;
	output.m128_f32[1] = input.y;
	output.m128_f32[2] = input.z;
	output.m128_f32[3] = 1.0f;
	return output;
}


inline float3 XMVectortofloat3(XMVECTOR input)
{
	float3 output;
	output.x = input.m128_f32[0];
	output.y = input.m128_f32[1];
	output.z = input.m128_f32[2];
	return output;
}

inline float DitanceFloat3(float3 pos1, float3 pos2)
{
	return sqrtf(pow((pos2.x - pos1.x), 2) + pow((pos2.y - pos1.y), 2) + pow((pos2.z - pos1.z), 2));
}

inline float DotProduct(float3 a, float3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline float MinFloat(float x, float y)
{
	if (x <= y)
	{
		return x;
	}

	return y;
}

inline float MaxFloat(float x, float y)
{
	if (x >= y)
	{
		return x;
	}

	return y;
}

struct AABB
{
	float3 center;
	float3 dimensions;
};

struct Ray
{
	float3 dir;
	float3 start;
};

struct Sphere
{
	float radius;
	float3 center;
};

inline float3 GetMin(AABB ab)
{
	return { ab.center.x - ab.dimensions.x, ab.center.y - ab.dimensions.y, ab.center.z - ab.dimensions.z };
}

inline float3 GetMax(AABB ab)
{
	return { ab.center.x + ab.dimensions.x, ab.center.y + ab.dimensions.y, ab.center.z + ab.dimensions.z };
}

inline bool SpheresCollide(Sphere a, Sphere b)
{
	float3 dist;
	dist.x = a.center.x - b.center.x;
	dist.y = a.center.y - b.center.y;
	dist.z = a.center.z - b.center.z;

	float length = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

	float sum = a.radius + b.radius;

	if (length <= sum)
	{
		return true;
	}

	return false;
}

inline bool RayToSphere(const Ray &ray, const Sphere &sphere, float &ft)
{
	float3 vecV1 = ray.start - sphere.center;

	float fb = DotProduct(vecV1, ray.dir);

	float fc = DotProduct(vecV1, vecV1) - (sphere.radius * sphere.radius);

	if (fc > 0.0f && fb > 0.0f)
	{
		return false;
	}

	float fDisc = fb * fb - fc;

	if (fDisc < 0.0f)
	{
		return false;
	}

	ft = -fb - sqrtf(fDisc);

	if (ft < 0.0f)
	{
		ft = 0.0f;
	}

	return true;
}

inline bool MovingSphereToSphere(const Sphere &moving, const float3 &velocity, const Sphere &Static, float &ft)
{

	Sphere temp = Static;
	temp.radius += moving.radius;

	float vecLength = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

	Ray tempR;
	tempR.start = moving.center;
	tempR.dir.x = velocity.x / vecLength;
	tempR.dir.y = velocity.y / vecLength;
	tempR.dir.z = velocity.z / vecLength;

	if (RayToSphere(tempR, temp, ft))
	{
		if (ft <= vecLength)
		{
			return true;
		}
	}

	return false;
}

inline bool SphereToAABB(Sphere s, AABB a)
{
	float3 Min = GetMin(a);
	float3 Max = GetMax(a);

	float x = MaxFloat(Min.x, MinFloat(s.center.x, Max.x));
	float y = MaxFloat(Min.y, MinFloat(s.center.y, Max.y));
	float z = MaxFloat(Min.z, MinFloat(s.center.z, Max.z));

	float distance = DitanceFloat3(float3{x, y, z}, s.center);

	return distance < s.radius;
}

inline bool RayToAABB(Ray r, AABB ab)
{
	float3 Min = GetMin(ab);
	float3 Max = GetMax(ab);

	float t1 = (Min.x - r.start.x) / r.dir.x;
	float t2 = (Max.x - r.start.x) / r.dir.x;
	float t3 = (Min.y - r.start.y) / r.dir.y;
	float t4 = (Max.y - r.start.y) / r.dir.y;
	float t5 = (Min.z - r.start.z) / r.dir.z;
	float t6 = (Max.z - r.start.z) / r.dir.z;

	float tmin = MaxFloat(MaxFloat(MinFloat(t1, t2), MinFloat(t3, t4)), MinFloat(t5, t6));
	float tmax = MinFloat(MinFloat(MaxFloat(t1, t2), MaxFloat(t3, t4)), MaxFloat(t5, t6));

	if (tmax < 0)
	{
		return false;
	}

	if (tmin > tmax)
	{
		return false;
	}

	return true;
}

inline bool pointCircle(float3 point, Sphere sphe)
{
	float distX = point.x - sphe.center.x;
	float distY = point.y - sphe.center.y;
	float distZ = point.z - sphe.center.z;
	float distance = sqrtf(pow(distX, 2) + pow(distY, 2) + pow(distZ, 2));

	if (distance <= sphe.radius)
	{
		return true;
	}

	return false;

}

inline float RandomUniform()
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = 0.2f - 1.0f;
	float r = random * diff;
	return 1.0f + r;
}



