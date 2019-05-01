#include "DebugClass.h"


DebugRenderer::DebugRenderer()
{
	DebugVBuffer = nullptr;
}

bool DebugRenderer::InitializeBuffer(ID3D11Device *device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(end::colored_vertex) * end::debug_renderer::get_line_vert_capacity();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = end::debug_renderer::get_line_verts();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &DebugVBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DebugRenderer::ShutdownBuffer()
{
	if (DebugVBuffer)
	{
		DebugVBuffer->Release();
		DebugVBuffer = 0;
	}
}

void DebugRenderer::RenderBuffer(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(end::colored_vertex);
	offset = 0;

	deviceContext->UpdateSubresource(DebugVBuffer, 0, nullptr, end::debug_renderer::get_line_verts(), 0, 0);

	deviceContext->IASetVertexBuffers(0, 1, &DebugVBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool DebugRenderer::Initialize(ID3D11Device *device)
{
	bool result;

	result = InitializeBuffer(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void DebugRenderer::Shutdown()
{
	ShutdownBuffer();
}

void DebugRenderer::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffer(deviceContext);
}

void DebugRenderer::MakeSphere(Sphere sphe)
{
	end::float3 points[6];

	points[0] = {sphe.center.x, sphe.center.y, sphe.center.z + sphe.radius};
	points[1] = {sphe.center.x, sphe.center.y, sphe.center.z - sphe.radius};
	points[2] = {sphe.center.x + sphe.radius, sphe.center.y, sphe.center.z};
	points[3] = {sphe.center.x - sphe.radius, sphe.center.y, sphe.center.z};
	points[4] = {sphe.center.x, sphe.center.y + sphe.radius, sphe.center.z};
	points[5] = {sphe.center.x, sphe.center.y - sphe.radius, sphe.center.z};

	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[0], {0, 0, 1, 1});
	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[1], {0, 0, 1, 1});
	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[2], {0, 0, 1, 1});
	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[3], {0, 0, 1, 1});
	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[4], {0, 0, 1, 1});
	end::debug_renderer::add_line({ sphe.center.x, sphe.center.y, sphe.center.z }, points[5], {0, 0, 1, 1});
}

void DebugRenderer::MakeAABB(AABB ab)
{
	end::float3 points[8];
	float minX = ab.center.x - ab.dimensions.x;
	float minY = ab.center.y - ab.dimensions.y;
	float minZ = ab.center.z - ab.dimensions.z;
	float maxX = ab.center.x + ab.dimensions.x;
	float maxY = ab.center.y + ab.dimensions.y;
	float maxZ = ab.center.z + ab.dimensions.z;
	points[0] = {minX, minY, minZ};
	points[1] = {minX, maxY, minZ};
	points[2] = {maxX, maxY, minZ};
	points[3] = {maxX, minY, minZ};
	points[4] = {maxX, minY, maxZ};
	points[5] = {maxX, maxY, maxZ};
	points[6] = {minX, maxY, maxZ};
	points[7] = {minX, minY, maxZ};

	end::debug_renderer::add_line(points[0], points[1], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[0], points[3], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[0], points[7], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[1], points[2], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[1], points[6], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[2], points[3], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[2], points[5], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[3], points[4], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[4], points[5], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[4], points[7], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[5], points[6], { 0, 1, 0, 1 });
	end::debug_renderer::add_line(points[6], points[7], { 0, 1, 0, 1 });
}

void DebugRenderer::AddLine(end::float3 pos1, end::float3 pos2, end::float4 color)
{
	end::debug_renderer::add_line(pos1, pos2, color);
}

int DebugRenderer::GetVertCount()
{
	return vertexCount;
}




