cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct InputVertex
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tan : TANGENT;
	float4 joints : JOINTS;
	float4 weights : WEIGHTS;
};

struct OutputVertex
{
	float4 FinalPosition : SV_POSITION;
	float3 FinalTexture : TEXCOORD0;
};

OutputVertex SkyboxVertex(InputVertex input)
{
	OutputVertex OUT = (OutputVertex)0;
	OUT.FinalPosition = mul(float4(input.position, 1.0f), World);
	OUT.FinalPosition = mul(OUT.FinalPosition, View);
	OUT.FinalPosition = mul(OUT.FinalPosition, Projection);
	OUT.FinalTexture =float4(input.position, 1.0f);
	return OUT;
}

