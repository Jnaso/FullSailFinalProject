#pragma once
#ifndef _DEBUGCLASS_H_
#define _DEBUGCLASS_H_

#include "MathDefines.h"
#include "debug_renderer.h"

class DebugRenderer
{
private:
	ID3D11Buffer *DebugVBuffer;
	int vertexCount;

	bool InitializeBuffer(ID3D11Device *);
	void ShutdownBuffer();
	void RenderBuffer(ID3D11DeviceContext *);

public:
	DebugRenderer();

	bool Initialize(ID3D11Device *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	void MakeSphere(Sphere);
	void MakeAABB(AABB);

	int GetVertCount();

};

#endif // !_DEBUGCLASS_H_
