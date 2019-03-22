#include "allofthelights.h"

Lighting::Lighting()
{

}

//Directional light color setter 
void Lighting::SetDirectionalColor(float red, float green, float blue, float alpha)
{
	DirectionalColor = { red, green, blue, alpha };
}

//Directional light direction setter 
void Lighting::SetDirection(float x, float y, float z)
{
	DirectionalDirection = { x, y, z };
}

//Spotlight color setter 
void Lighting::SetSpotlightColor(float r, float g, float b, float a)
{
	SpotlightColor = { r, g, b, a };
}

//Spotlight direction setter 
void Lighting::SetSpotlightDirection(float x, float y, float z, float w)
{
	SpotlightDirection = { x, y, z, w };
}

//Spotlight position setter 
void Lighting::SetSpotlightPosition(float x, float y, float z, float w)
{
	SpotlightPosition = { x, y, z, w };
}

//Spotlight extra setter 
void Lighting::SetSpotlightExtra(float a, float b, float c, float d)
{
	SpotlightExtra = { a, b, c, d };
}

void Lighting::SetSpecularColor(float r, float g, float b, float a)
{
	SpecularColor = { r, g, b, a };
}

void Lighting::SetSpecularExtra(float a, float b, float c, float d)
{
	SpecularExtra = { a, b, c, d };
}

//Directional light color getter 
XMFLOAT4 Lighting::GetDirectionalColor()
{
	return DirectionalColor;
}

//Directional light direction getter 
XMFLOAT3 Lighting::GetDirectionalDirection()
{
	return DirectionalDirection;
}

//Spotlight color getter 
XMFLOAT4 Lighting::GetSpotlightColor()
{
	return SpotlightColor;
}

//Spotlight direction getter 
XMFLOAT4 Lighting::GetSpotlightDirection()
{
	return SpotlightDirection;
}

//Spotlight position getter 
XMFLOAT4 Lighting::GetSpotlightPosition()
{
	return SpotlightPosition;
}

//Spotlight extra getter 
XMFLOAT4 Lighting::GetSpotlightExtra()
{
	return SpotlightExtra;
}

XMFLOAT4 Lighting::GetSpecularColor()
{
	return SpecularColor;
}

XMFLOAT4 Lighting::GetSpecularExtra()
{
	return SpecularExtra;
}