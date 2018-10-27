//-----------------------------------------------------------------------------
//  Main.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Main File -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

CCore::CCore()
{
	prv_EngineLogFile	= new CLogFile("lf3dlogfile.txt");
	prv_GraphicDevice 	= new CGraphicDevice(this);
	prv_InputDevice		= new CInputDevice(this);
	prv_AudioDevice		= new CAudioDevice(this);
	prv_CameraDevice 	= new CCameraDevice(this);
	prv_FontLoader		= new CFontLoader(this);
	prv_ImageLoader		= new CImageLoader(this);
	prv_ModelLoader		= new CModelLoader(this);
	prv_TerrainLoader	= new CTerrainLoader(this);
	prv_SceneGraph		= new CSceneGraph(this);
	prv_NetworkClient	= new CNetworkClient(this);
	prv_NetworkServer	= new CNetworkServer(this);

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
		prv_EngineLogFile->WriteText("Error: Init Video");

	if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
		prv_EngineLogFile->WriteText("Error: Init Timer");
}

CLogFile* CCore::GetLogFile()
{
	return prv_EngineLogFile;
}

CGraphicDevice* CCore::GetGraphicDevice()
{
	return prv_GraphicDevice;
}

CInputDevice* CCore::GetInputDevice()
{
	return prv_InputDevice;
}

CAudioDevice* CCore::GetAudioDevice()
{
	return prv_AudioDevice;
}

CCameraDevice* CCore::GetCameraDevice()
{
	return prv_CameraDevice;
}

CFontLoader* CCore::GetFontLoader()
{
	return prv_FontLoader;
}

CImageLoader* CCore::GetImageLoader()
{
	return prv_ImageLoader;
}

CModelLoader* CCore::GetModelLoader()
{
	return prv_ModelLoader;
}

CTerrainLoader* CCore::GetTerrainLoader()
{
	return prv_TerrainLoader;
}

CSceneGraph* CCore::GetSceneGraph()
{
	return prv_SceneGraph;
}

CNetworkClient* CCore::GetNetworkClient()
{
	return prv_NetworkClient;
}

CNetworkServer* CCore::GetNetworkServer()
{
	return prv_NetworkServer;
}

CCore::~CCore()
{
	SAFE_DELETE(prv_NetworkServer);
	SAFE_DELETE(prv_NetworkClient);
	SAFE_DELETE(prv_SceneGraph);
	SAFE_DELETE(prv_TerrainLoader);
	SAFE_DELETE(prv_ModelLoader);
	SAFE_DELETE(prv_ImageLoader);
	SAFE_DELETE(prv_FontLoader);
	SAFE_DELETE(prv_CameraDevice);
	SAFE_DELETE(prv_AudioDevice);
	SAFE_DELETE(prv_InputDevice);
	SAFE_DELETE(prv_GraphicDevice);
	SAFE_DELETE(prv_EngineLogFile);
}

};
