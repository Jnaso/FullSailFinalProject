#pragma once

#include <vector>
#include <array>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

using namespace DirectX;

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
	inline float3& operator+=(float3 b)
	{
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		return *this;
	}
	inline const float3& operator+=(float3 b)const
	{
		float3 newVal;
		newVal.x = this->x + b.x;
		newVal.y = this->y + b.y;
		newVal.z = this->z + b.z;
		return newVal;
	}
	inline float3& operator-=(float3 b)
	{
		this->x -= b.x;
		this->y -= b.y;
		this->z -= b.z;
		return *this;
	}
	inline const float3& operator-=(float3 b)const
	{
		float3 newVal;
		newVal.x = this->x - b.x;
		newVal.y = this->y - b.y;
		newVal.z = this->z - b.z;
		return newVal;
	}
	inline float3& operator-=(float b)
	{
		this->x -= b;
		this->y -= b;
		this->z -= b;
		return *this;
	}
	inline const float3& operator-=(float b)const
	{
		float3 newVal;
		newVal.x = this->x - b;
		newVal.y = this->y - b;
		newVal.z = this->z - b;
		return newVal;
	}

	inline float3& operator*(float val)
	{
		this->x *= val;
		this->y *= val;
		this->z *= val;
		return *this;
	}

	inline const float3& operator*(float val)const
	{
		float3 newVal;
		newVal.x = this->x *val;
		newVal.y = this->y *val;
		newVal.z = this->z *val;
		return newVal;
	}

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

