//#pragma pack_major(row_major)
#define NUM_LIGHTS 2

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
	float3 PixelPos : TEXCOORD1;
};

PixelInput Main(VertexInput input)
{
	PixelInput output = (PixelInput)0;
	float4 pixelPosition;

	float4 skinned = float4(0, 0, 0, 1);
	float4 skinnedNorm = float4(0, 0, 0, 0);

	for(int i = 0; i < 4; i ++)
	{
		skinned += mul(float4(input.pos, 1.0f), bindPoses[input.joints[i]]) * input.weights[i];
		skinnedNorm += mul(float4(input.norm, 0.0f), bindPoses[input.joints[i]]) * input.weights[i];
	}

	output.pos = skinned;
	output.pos = mul(output.pos, worldMatrix);
	output.PixelPos = output.pos;
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.tex = input.tex;

	output.norm = mul(float4(skinnedNorm.xyz, 0), worldMatrix).xyz;
	output.Tang = normalize(mul(float4(input.tan, 0), worldMatrix).xyz);
	output.Binomial = cross(output.norm, output.Tang);

	// = mul(float4(input.pos, 1.0f), worldMatrix).xyz;

	return output;
}