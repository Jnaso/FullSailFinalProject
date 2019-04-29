#pragma once

#include "Math_Types.h"

// Interface to the debug renderer
namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b);

		inline void add_line(float3 p, float3 q, float4 color) { add_line(p, q, color, color); }

		void clear_lines();

		void DrawAABB(float3 p1, float3 p2, float3 p3, float3 p4, float3 p5, float3 p6, float3 p7, float3 p8, float4 color);

		const colored_vertex* get_line_verts();

		size_t get_line_vert_count();

		size_t get_line_vert_capacity();
	}
}