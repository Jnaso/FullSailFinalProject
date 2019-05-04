#include "Frustum.h"

Frustum::Frustum()
{

}

void Frustum::Construct(float screenDepth, XMMATRIX projection, XMMATRIX view)
{
	float zMinimum, r;
	XMMATRIX matrix = XMMatrixIdentity();
	XMMATRIX proj = XMMatrixIdentity();
	XMMATRIX temp = XMMatrixIdentity();

	zMinimum = -projection.r[3].m128_f32[2] / projection.r[2].m128_f32[2];
	r = screenDepth / (screenDepth - zMinimum);
	projection.r[2].m128_f32[2] = r;
	projection.r[3].m128_f32[2] = -r * zMinimum;

	matrix = XMMatrixMultiply(view, projection);

	myPlanes[0].x = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[2];
	myPlanes[0].y = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[2];
	myPlanes[0].z = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[2];
	myPlanes[0].w = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[2];
	XMStoreFloat4(&myPlanes[0], XMPlaneNormalize(XMLoadFloat4(&myPlanes[0])));

	myPlanes[1].x = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[2];
	myPlanes[1].y = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[2];
	myPlanes[1].z = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[2];
	myPlanes[1].w = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[2];
	XMStoreFloat4(&myPlanes[1], XMPlaneNormalize(XMLoadFloat4(&myPlanes[1])));

	myPlanes[2].x = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[0];
	myPlanes[2].y = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[0];
	myPlanes[2].z = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[0];
	myPlanes[2].w = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[0];
	XMStoreFloat4(&myPlanes[2], XMPlaneNormalize(XMLoadFloat4(&myPlanes[2])));

	myPlanes[3].x = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[0];
	myPlanes[3].y = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[0];
	myPlanes[3].z = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[0];
	myPlanes[3].w = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[0];
	XMStoreFloat4(&myPlanes[3], XMPlaneNormalize(XMLoadFloat4(&myPlanes[3])));

	myPlanes[4].x = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[1];
	myPlanes[4].y = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[1];
	myPlanes[4].z = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[1];
	myPlanes[4].w = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[1];
	XMStoreFloat4(&myPlanes[4], XMPlaneNormalize(XMLoadFloat4(&myPlanes[4])));

	myPlanes[5].x = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[1];
	myPlanes[5].y = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[1];
	myPlanes[5].z = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[1];
	myPlanes[5].w = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[1];
	XMStoreFloat4(&myPlanes[5], XMPlaneNormalize(XMLoadFloat4(&myPlanes[5])));
}

bool Frustum::CheckSphere(Sphere sphe)
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&myPlanes[i]), XMVectorSet(sphe.center.x, sphe.center.y, sphe.center.z, 1.0f))) < -sphe.radius)
		{
			return false;
		}
	}

	return true;
}
