cbuffer MVP_t
{
	matrix modeling;
	matrix view;
	matrix projection;
};

struct VSIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct PS_OUTPUT
{
	float4 finalPos : SV_POSITION;
	float4 finalColor : COLOR;
};

PS_OUTPUT main(VSIn IN)
{
	PS_OUTPUT OUT = (PS_OUTPUT)0;
	OUT.finalPos = float4(IN.pos.xyz, 1.0f);
	OUT.finalPos = mul(OUT.finalPos, modeling);
	OUT.finalPos = mul(OUT.finalPos, view);
	OUT.finalPos = mul(OUT.finalPos, projection);
	OUT.finalColor = IN.color;

	return OUT;
}