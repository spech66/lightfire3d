//-----------------------------------------------------------------------------
//  InputDevice.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle Input -
//-----------------------------------------------------------------------------

#ifndef __LF3DINPUTDEVICEH__
#define __LF3DINPUTDEVICEH__

namespace LF3D
{

	class CInputDevice
	{
		private:
			//! Pointer to CoreDevice
			CCore			*prv_Core;
			//! Contains the state of every key on the keyboard
			Uint8			prv_Keyboard[323];
			//! The x and y coordinates of the mouse
			int				prv_MouseX, prv_MouseY;
			//! Contains the state of every mouse button
			bool			prv_MouseB[8];
			//! Contains the user's key presses as readable text
			string			prv_TextInput;
			
		public:		
			//! Constructor
			/*!
				Init all Vars
				\sa CInputDevice()
			*/
					CInputDevice(CCore *Core);
			//! Destructor
			/*!
				\sa ~CInputDevice()
			*/
					~CInputDevice();			
			//! UpdateInput
			/*!
				Handle Input Events
				\sa UpdateInput()
			*/			
			void	UpdateInput();
			
			bool	GetKeyState(SDLKey Key);
			bool	GetMouseButtonState(int Button);
			void	GetMousePos(int &X, int &Y);
			string	GetTextInput();
	};

};

#endif
