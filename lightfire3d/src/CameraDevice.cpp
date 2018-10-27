//-----------------------------------------------------------------------------
//  CameraDevice.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Camera -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		init
// Description:	Init Camera
//-----------------------------------------------------------------------------
CCameraDevice::CCameraDevice(CCore *Core)
{
	prv_Core = Core;
	prv_Position = Vector3(0.0f, 0.0f, 0.0f);
	prv_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	prv_Distance = 1.0f;
}

//-----------------------------------------------------------------------------
// Name:		UpdateCamera
// Description:	Update Camera
//-----------------------------------------------------------------------------
void CCameraDevice::UpdateObjectAlignedCamera()
{
	Vector3 vCameraPosition = prv_Position;

	vCameraPosition.x = prv_Distance * sin(prv_Rotation.y * PI / 180.0f) + prv_Position.x;
	vCameraPosition.y = prv_Distance * sin(prv_Rotation.z * PI / 180.0f) + prv_Position.y;
	vCameraPosition.z = prv_Distance * cos(prv_Rotation.y * PI / 180.0f) + prv_Position.z;

	gluLookAt(vCameraPosition.x, vCameraPosition.y, vCameraPosition.z,
			  prv_Position.x, prv_Position.y, prv_Position.z, 0, 1, 0);
}

//-----------------------------------------------------------------------------
// Name:		MoveCamera
// Description:	Move Camera
//-----------------------------------------------------------------------------
void CCameraDevice::MoveCamera(float x, float y, float z)
{
	prv_Position.x += x;
	prv_Position.y += y;
	prv_Position.z += z;
}

//-----------------------------------------------------------------------------
// Name:		RotateCamera
// Description:	Rotate Camera
//-----------------------------------------------------------------------------
void CCameraDevice::RotateCamera(float x, float y, float z)
{
	prv_Rotation.x += x;
	prv_Rotation.y += y;
	prv_Rotation.z += z;
	if(prv_Rotation.x > 360) prv_Rotation.x -= 360.0f;
	if(prv_Rotation.y > 360) prv_Rotation.y -= 360.0f;
	if(prv_Rotation.z > 360) prv_Rotation.z -= 360.0f;
}

//-----------------------------------------------------------------------------
// Name:		SetPosition
// Description:	Set Camera Position
//-----------------------------------------------------------------------------
void CCameraDevice::SetPosition(float x, float y, float z)
{
	prv_Position.x = x;
	prv_Position.y = y;
	prv_Position.z = z;
}

//-----------------------------------------------------------------------------
// Name:		SetRotation
// Description:	Set Camera Rotation
//-----------------------------------------------------------------------------
void CCameraDevice::SetRotation(float x, float y, float z)
{
	prv_Rotation.x = x;
	prv_Rotation.y = y;
	prv_Rotation.z = z;
}

//-----------------------------------------------------------------------------
// Name:		ZoomCamera
// Description:	Zoom Camera
//-----------------------------------------------------------------------------
void CCameraDevice::ZoomCamera(float z)
{
	prv_Distance += z;
	if(prv_Distance < 0.01f) prv_Distance = 0.01f;
}

//-----------------------------------------------------------------------------
// Name:		ZoomCamera
// Description:	Zoom Camera(absoulte)
//-----------------------------------------------------------------------------
void CCameraDevice::ZoomCameraTo(float z)
{
	prv_Distance = z;
	if(prv_Distance < 0.01f) prv_Distance = 0.01f;
}

//-----------------------------------------------------------------------------
// Name:		GetPosition
// Description:	Get Position
//-----------------------------------------------------------------------------
Vector3	CCameraDevice::GetPosition()
{
	return prv_Position;
}

//-----------------------------------------------------------------------------
// Name:		GetRotation
// Description:	Get Rotation
//-----------------------------------------------------------------------------
Vector3 CCameraDevice::GetRotation()
{
	return prv_Rotation;
}

//-----------------------------------------------------------------------------
// Name:		GetZoom
// Description:	Get Zoom
//-----------------------------------------------------------------------------
float CCameraDevice::GetZoom()
{
	return prv_Distance;
}

};
