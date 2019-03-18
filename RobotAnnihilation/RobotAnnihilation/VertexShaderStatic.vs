//#pragma pack_major(row_major)

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
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

	input.position.w = 1.0f;

	output.pos = mul(input.position, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.tex = input.tex;

	output.norm = mul(input.normal, worldMatrix);
	output.norm = normalize(output.norm);
	
	output.PixelPos = mul(input.position, worldMatrix).xyz;

	return output;
}