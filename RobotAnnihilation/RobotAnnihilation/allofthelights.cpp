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