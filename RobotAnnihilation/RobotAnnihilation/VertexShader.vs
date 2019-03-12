cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL;
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

	out.pos = mul(in.pos, worldMatrix);
	out.pos = mul(out.pos, viewMatrix);
	out.pos = mul(out.pos, projectionMatrix);

	out.tex = in.tex;

	out.norm = mul(in.norm, worldMatrix);
	out.norm = normalize(out.norm);

	return out;
}