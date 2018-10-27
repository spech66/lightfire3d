//-----------------------------------------------------------------------------
//  CameraDevice.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Camera -
//-----------------------------------------------------------------------------
#ifndef __LF3DCAMERADEVICEH__
#define __LF3DCAMERADEVICEH__

namespace LF3D
{

//-----------------------------------------------------------------------------
// Deklaration
//-----------------------------------------------------------------------------
	class CCameraDevice
	{
		private:
			//! Pointer to CoreDevice
			CCore		*prv_Core;
			//! The camera's position
			Vector3		prv_Position;
			//! The camera's alignment
			Vector3 	prv_Rotation;
			//! The camera's zoom factor
			float		prv_Distance;

		public:
			//! Constructor
			/*!
				Places the camera at (0,0,0).
			*/
						CCameraDevice(CCore *Core);
			//! Activates this camera
			/*!
				Actually, this uses gluLookAt to apply the camera's data to the projection matrix.
				You'll need to call this function to apply any changes you may have made to the camera.
			*/
			void		UpdateObjectAlignedCamera();
			
			//! Moves the camera by (x,y,z) units
			void		MoveCamera(float x, float y, float z);

			//! Rotates the camera by (x,y,z) degrees
			void		RotateCamera(float x, float y, float z);

			//! Moves the camera to a certain position
			void		SetPosition(float x, float y, float z);
			//! Rotates the camera to a certain alignment
			void		SetRotation(float x, float y, float z);
			//! This increases/decreases the camera's zoom by z
			/*!	
				Since this changes the cameras distance from the Target,
				zooming is reversed, the actual zoom factor is (1.0/CameraZoom).
				For example, if you have an unzoomed camera, and you call ZoomCamera(1.0),
				the distance between the camera and the target object will double
				('cause the new zoom is 1.0 + 1.0 = 2.0) 
				\sa ZoomCameraTo()
			*/
			void		ZoomCamera(float z);
			//! This changes the camera's zoom factor to z
			/*!
				Since this changes the cameras distance from the Target,
				zooming is reversed, the actual zoom factor is (1.0/CameraZoom).
				For example, if you have an unzoomed camera, and you call ZoomCamera(2.0),
				the distance between the camera and the target object will double
				('cause the new zoom is 2.0) 
				\sa ZoomCamera()
			*/
			void 	ZoomCameraTo(float z);
			//! Returns the camera's absolute coordinates
			/*!
				\return The camera's absolute coordinates.
			*/
			Vector3		GetPosition();
			//! Returns the camera's alignment
			/*!
				\return The camera's alignment.
			*/
			Vector3		GetRotation();
			//! Returns the camera's zoom
			/*!
				\return The camera's zoom.
			*/
			float		GetZoom();
	};

};

#endif
