#include "debug_renderer.h"
#include <array>

// Anonymous namespace
namespace
{
	// Declarations in an anonymous namespace are global BUT only have internal linkage.
	// In other words, these variables are global but are only visible in this source file.

	// Maximum number of debug lines at one time (i.e: Capacity)
	constexpr size_t MAX_LINE_VERTS = 4096;

	// CPU-side buffer of debug-line verts
	// Copied to the GPU and reset every frame.
	size_t line_vert_count = 0;
	std::array< colored_vertex, MAX_LINE_VERTS> line_verts;
}

//namespace end
//{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b)
		{
			// Add points to debug_verts, increments debug_vert_count
			colored_vertex a;
			a.pos = point_a;
			a.color = color_a;
			line_verts[line_vert_count] = a;
			line_vert_count++;
			colored_vertex b;
			b.pos = point_b;
			b.color = color_b;
			line_verts[line_vert_count] = b;
			line_vert_count++;
		}

		void DrawAABB(float3 p1, float3 p2, float3 p3, float3 p4, float3 p5, float3 p6, float3 p7, float3 p8, float4 color)
		{
			add_line(p1, p2, color);
			add_line(p1, p8, color);
			add_line(p1, p3, color);
			add_line(p2, p7, color);
			add_line(p2, p4, color);
			add_line(p4, p6, color);
			add_line(p4, p3, color);
			add_line(p3, p5, color);
			add_line(p5, p6, color);
			add_line(p5, p8, color);
			add_line(p8, p7, color);
			add_line(p7, p6, color);
		}

		void clear_lines()
		{
			// Resets debug_vert_count
			line_vert_count = 0;
		}

		const colored_vertex* get_line_verts()
		{
			// Does just what it says in the name
			return line_verts.data();
		}

		size_t get_line_vert_count()
		{
			// Does just what it says in the name
			return line_vert_count;
		}

		size_t get_line_vert_capacity()
		{
			// Does just what it says in the name
			return MAX_LINE_VERTS;
		}
	}
//}