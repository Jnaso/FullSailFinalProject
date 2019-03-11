#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{

}


Camera::~Camera()
{
}

void Camera::Update(float delta)
{
	
}
//
//void UpdateMousePos()
//{
//	deltax = 0;
//	deltay = 0;
//	deltax = current.x - prev.x;
//	deltay = current.y - prev.y;
//	prev = current;

//CamCopy = GW::MATH::(CamCopy, XMMatrixTranslation(xaxischange, yaxischange, zaxischange));
//GW::MATH::GVECTORF camerapos = CamCopy.row4;
//CamCopy.r[3].m128_f32[0] = CamCopy.r[3].m128_f32[1] = CamCopy.r[3].m128_f32[2] = 0;
//CamCopy = XMMatrixMultiply(CamCopy, XMMatrixRotationX(deltay * delta * -.7f));
//CamCopy = XMMatrixMultiply(XMMatrixRotationY(deltax * delta * -.7f), CamCopy);
//CamCopy.r[3].m128_f32[0] = camerapos.m128_f32[0];
//CamCopy.r[3].m128_f32[1] = camerapos.m128_f32[1];
//CamCopy.r[3].m128_f32[2] = camerapos.m128_f32[2];
//tempView = CamCopy;
//}