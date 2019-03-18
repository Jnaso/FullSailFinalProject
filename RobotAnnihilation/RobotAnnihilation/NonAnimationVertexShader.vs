//#pragma pack_major(row_major)

cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4x4 bindPoses[28];
};

struct VertexInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tan : TANGENT;
	int4 joints : JOINTS;
	float4 weights : WEIGHTS;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL;
	float3 Tang : TANGENT;
	float3 Binomial : BINOMIAL;
};

PixelInput Main(VertexInput input)
{
	PixelInput output = (PixelInput)0;

	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(output.pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.tex = input.tex;

	output.norm = mul(float4(input.norm, 0.0f), worldMatrix);
	output.norm = normalize(output.norm);
	output.Tang = normalize(mul(float4(input.tan, 0), worldMatrix).xyz);
	output.Binomial = cross(output.norm, output.Tang);

	return output;
}