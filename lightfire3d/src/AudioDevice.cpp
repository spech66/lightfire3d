//-----------------------------------------------------------------------------
//  AudioDevice.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle Audiere -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CAudioDevice
// Description:	Constructor
//-----------------------------------------------------------------------------
CAudioDevice::CAudioDevice(CCore *Core)
{
	prv_Core = Core;
}

//-----------------------------------------------------------------------------
// Name:		Init
// Description:	Init Audio Device
//-----------------------------------------------------------------------------
RESULT CAudioDevice::Init()
{
	prv_Core->GetLogFile()->WriteText("Init: --> AudioDevice");
	
	prv_Device = NULL;
	prv_Device = OpenDevice();
	if (!prv_Device.get())
	{
		prv_Core->GetLogFile()->WriteText("Error: Open AudioDevice.");
		prv_Core->GetLogFile()->WriteText("Init: Trying NULL Device now");
	
		prv_Device = OpenDevice("null");
		if (!prv_Device)
		{
			prv_Core->GetLogFile()->WriteText("Error: Open AudioDevice");
			return ERROR;
		}
	}

	prv_Core->GetLogFile()->WriteText("Init: <-- AudioDevice");
	return OK;
}

//-----------------------------------------------------------------------------
// Name:		LoadSound
// Description:	Load sound from file
//-----------------------------------------------------------------------------
RESULT CAudioDevice::LoadSound(string FileName, bool Stream)
{
	if(prv_Sounds[FileName])
	{
		prv_Core->GetLogFile()->WriteText("Init: LoadSound(%s) already loaded", FileName.c_str());
		return OK;
	}
	
	if(!prv_Device)
		return ERROR;

	OutputStreamPtr TempSound;
	TempSound = OpenSound(prv_Device, FileName.c_str(), Stream);
	if (!TempSound)
	{
		prv_Core->GetLogFile()->WriteText("Error: LoadSound(%s)", FileName.c_str());		
		return ERROR;
	}

	prv_Sounds[FileName] = TempSound;
	prv_Core->GetLogFile()->WriteText("Init: LoadImage(%s)", FileName.c_str());

	return OK;
}

//-----------------------------------------------------------------------------
// Name:		PlaySound
// Description:	Play sound with volume - Loop for ambient sounds
//-----------------------------------------------------------------------------
void CAudioDevice::PlaySound(string FileName, float Volume, bool Loop)
{
	if(!prv_Sounds[FileName])
		return;
		
	if(!prv_Device)
		return;

	prv_Sounds[FileName]->setRepeat(Loop);
	prv_Sounds[FileName]->setVolume(Volume);
	prv_Sounds[FileName]->play();
}

//-----------------------------------------------------------------------------
// Name:		StopSound
// Description:	Stop running sound
//-----------------------------------------------------------------------------
void CAudioDevice::StopSound(string FileName)
{
	if(!prv_Sounds[FileName])
		return;
		
	if(!prv_Device)
		return;
		
	prv_Sounds[FileName]->stop();		
}

//-----------------------------------------------------------------------------
// Name:		FreeSound
// Description:	Free sound
//-----------------------------------------------------------------------------
void CAudioDevice::FreeSound(string FileName)
{
	if(!prv_Sounds[FileName])
		return;

	FreeSound(FileName.c_str());
	prv_Sounds.erase(FileName);
}
	
};
