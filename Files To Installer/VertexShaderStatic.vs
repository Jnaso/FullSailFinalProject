//#pragma pack_major(row_major)

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tan : TANGENT;
	float4 joints : JOINTS;
	float4 weights : WEIGHTS;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL;
	float3 Tang : TANGENT;
	float3 Binomial : BINOMIAL;
	float3 PixelPos : TEXCOORD1;
};

PixelInput Main(VertexInputType input)
{
	PixelInput output;

	output.pos = mul(float4(input.position, 1.0f), worldMatrix);
	output.PixelPos = output.pos;
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.tex = input.tex;
	
	output.norm = mul(float4(input.normal, 0), worldMatrix).xyz;
	output.Tang = normalize(mul(float4(input.tan, 0), worldMatrix).xyz);
	output.Binomial = cross(output.norm, output.Tang);
	
	// = mul(float4(input.position, 1.0f), worldMatrix).xyz;

	return output;
}