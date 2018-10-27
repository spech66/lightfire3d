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
#include "main.h"

//-----------------------------------------------------------------------------
// main - Program entry point
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//Create a core instance and get all Devices
	LF3D::CCore 			Core;
	LF3D::CGraphicDevice 	*GDevice 		= Core.GetGraphicDevice();
	LF3D::CCameraDevice 	*CamDevice 		= Core.GetCameraDevice();
	LF3D::CInputDevice 		*InputDevice 	= Core.GetInputDevice();
	LF3D::CImageLoader 		*ImageLoader 	= Core.GetImageLoader();
	LF3D::CModelLoader		*ModelLoader 	= Core.GetModelLoader();
	LF3D::CFontLoader		*FontLoader 	= Core.GetFontLoader();
	LF3D::CTerrainLoader	*TerrainLoader 	= Core.GetTerrainLoader();
	
	//Create a window
	GDevice->Init(1,	//Window Mode = 1; Fullscreen = 0;
				640,	//Window Width
				480,	//Window Height
				 32);	//Color depth
	GDevice->SetWindowText("Lightfire3D - Sample"); //Set Window Text
	
	//Load a font - Windows and Linux use very different Font names!
	string Font = "";
	#if defined(LF3DPLATFORM_LINUX)
		Font = "-*-helvetica-*-*-*-*-*-180-*-*-*-*-*-*";
		FontLoader->LoadFont(Font);		//Linux font name includes size
	#elif  defined(LF3DPLATFORM_WINDOWS)
		Font = "Arial";
		FontLoader->LoadFont(Font, 18);	//LoadFont(Font) would load a font with size 12
	#endif
	
	ModelLoader->LoadModel("player.3ds");				//Load a sample Model
	ModelLoader->SetPosition(0.0f, 40.0f, 0.0f);		//Set the Position
	ImageLoader->LoadImage("terrain.tga");				//Load Terrain Texture
	TerrainLoader->LoadTerrain("terrain.raw");			//Load 128*128 Terrain
	TerrainLoader->SetPosition(-128.0f, 0.0f, -128.0f); //Center the Terrain

	CamDevice->MoveCamera(0.0f, 40.0f, 0.0f);			//Set Camera to Position
	CamDevice->RotateCamera(45.0f, 0.0f, 45.0f);		//Rotate the Camera
	CamDevice->ZoomCamera(50.0f);						//Zoom out
	
	unsigned long FrameTime = SDL_GetTicks();
	unsigned long DeltaTime = 0;
	string CurTex = "Text: "; 	//We will save our sample Input Text here
	glDisable(GL_LIGHTING);		//Disable Lighting
	
	while(GDevice->IsRunning())	//Loop until we receive a Quit Message
	{
		DeltaTime = SDL_GetTicks() - FrameTime;	//Calculate FrameTime for Movement
		FrameTime = SDL_GetTicks();
		
		InputDevice->UpdateInput();	//Update Input
		if(InputDevice->GetKeyState(SDLK_ESCAPE)) GDevice->Quit(); 	//Quit on Escape
		int TempMouseX, TempMouseY;
		InputDevice->GetMousePos(TempMouseX, TempMouseY);			//Get Mouse Position
		CurTex += InputDevice->GetTextInput();					//Save text we entered
		
		GDevice->BeginRender();									//Enter Render Loop
		GDevice->SetPerspectiveView();							//Switch to 3D Mode
		CamDevice->RotateCamera(0.0f, DeltaTime*0.01f, 0.0f); 	//Rotate
		CamDevice->UpdateObjectAlignedCamera();					//Set Camera to new Pos
		
		ModelLoader->RenderModel("player.3ds");				//Render our sample model
		ImageLoader->SetTexture("terrain.tga");				//Set terrain texture
		TerrainLoader->RenderTerrain("terrain.raw");		//and render the terrain
		
		GDevice->SetOrthoView();					//Switch to 2D Mode
		glColor3ub(255, 255, 255);					//White Font Color
		FontLoader->SetPosition(20, 240);
		FontLoader->PrintText(Font, CurTex);		//Print our Message
		FontLoader->SetPosition(20, 20);
		char Curfps[128];
		sprintf(Curfps, "FPS: %i - Mouse: %i - %i", GDevice->GetFPS(), TempMouseX, TempMouseY);
		FontLoader->PrintText(Font, Curfps);		//Print some Messages
		
		GDevice->EndRender();						//End render and present Scene
	}

	return 0;
}
