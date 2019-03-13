cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

PixelInput VertexShader(VertexInput in)
{
	PixelInput out;

	out.pos = float4(mul(in.pos, worldMatrix).xyz, 1.0f);
	out.pos = mul(out.pos, viewMatrix);
	out.pos = mul(out.pos, projectionMatrix);

	out.tex = in.tex;

	out.norm = mul(in.norm, worldMatrix);
	out.norm = normalize(out.norm);

	return out;
}