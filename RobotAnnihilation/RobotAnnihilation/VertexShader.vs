cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4x4 bindPoses[50];
};

struct VertexInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tan : TANGENT;
	float4 joints : JOINTS;
	float4 weights : WEIGHTS;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL;
};

PixelInput Main(VertexInput input)
{
	PixelInput output;

	float4 skinned = float4(0, 0, 0, 0);
	float4 skinnedNorm = float(0, 0, 0, 0);

	for(int i = 0; i < 4; i ++)
	{
		skinned += mul(input.pos, bindPoses[input.joints[i]]) * input.weights[i];
		skinnedNorm += mul(input.norm, bindPoses[input.joints[i]]) * input.weights[i];
	}

	output.pos = float4(mul(input.pos, worldMatrix).xyz, 1.0f);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.tex = input.tex;

	output.norm = mul(input.norm, worldMatrix);
	output.norm = normalize(output.norm);

	return output;
}