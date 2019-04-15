#pragma once
#ifndef _MATH_TYPES_H_
#define _MATH_TYPES_H_

#include <array>
#include <math.h>
#include <cstdint>
#include <iostream> 
#include <vector>

using namespace std;

namespace end
{
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

	struct alignas(8) float2_a : float2 {};

	struct alignas(16) float3_a : float3 {};

	struct alignas(16) float4_a : float4 {};

	using float4x4 = std::array< float4, 4 >;
	using float4x4_a = std::array< float4_a, 4 >;

}

namespace end
{
	struct colored_vertex
	{
		float3 pos = { 0.0f, 0.0f, 0.0f };
		float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		colored_vertex() = default;
		colored_vertex(const colored_vertex&) = default;

		inline colored_vertex(const float3& p, const float4& c) : pos{ p }, color{ c } {}
		inline colored_vertex(const float3& p, const float3& c) : pos{ p }, color{ c.x, c.y, c.z, 1.0f } {}
		inline colored_vertex(const float3& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : pos{ p }, color{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f } {}
	};

	template<typename T>
	void Swap(T &a, T &b)
	{
		T c = a;
		a = b;
		b = c;
	}
}

#endif // !_MATH_TYPES_H_