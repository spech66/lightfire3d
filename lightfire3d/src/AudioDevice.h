//-----------------------------------------------------------------------------
//  AudioDevice.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle Audiere -
//-----------------------------------------------------------------------------

#ifndef __LF3DAUDIODEVICEH__
#define __LF3DAUDIODEVICEH__
using namespace audiere;

namespace LF3D
{

	class CAudioDevice
	{
		private:
			//! Pointer to CoreDevice
			CCore						*prv_Core;
			AudioDevicePtr				prv_Device;
			map<string, OutputStreamPtr>prv_Sounds;

		public:
					CAudioDevice(CCore *Core);
			RESULT	Init();
			RESULT	LoadSound(string FileName, bool Stream);
			void	PlaySound(string FileName, float Volume, bool Loop);
			void	StopSound(string FileName);
			void	FreeSound(string FileName);
	};

};

#endif
