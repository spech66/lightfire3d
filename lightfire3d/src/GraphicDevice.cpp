//-----------------------------------------------------------------------------
//  GraphicDevice.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle OpenGL and SDL -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CGraphicDevice
// Description:	Contructor
//-----------------------------------------------------------------------------
CGraphicDevice::CGraphicDevice(CCore *Core)
{
	prv_Core = Core;
	prv_FrameTime = SDL_GetTicks();
	prv_DeltaTime = 0;
}

//-----------------------------------------------------------------------------
// Name:		init
// Description:	Init of Graphic Stuff
//-----------------------------------------------------------------------------
RESULT CGraphicDevice::Init(bool bWindowed, int Width, int Height, int ColorDepth)
{
	prv_Core->GetLogFile()->WriteText("Init: --> GraphicDevice");
	
    const SDL_VideoInfo *VideoInfo;
	SDL_Surface         *SDLSurface;
	prv_WindowHeight	= Height;
	prv_WindowWidth		= Width;
	prv_ColorDepth 		= ColorDepth;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	prv_VideoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE;

	if(!bWindowed)
	{
		prv_VideoFlags |= SDL_FULLSCREEN | SDL_ANYFORMAT;
		prv_Core->GetLogFile()->WriteText("Init: Switching to Fullscreen");
	} else {
		prv_Core->GetLogFile()->WriteText("Init: Switching to Window");
	}

	VideoInfo = SDL_GetVideoInfo();
	if (!VideoInfo)
	{
		prv_Core->GetLogFile()->WriteText("Error: Get video info");
		return SDL_ERR_VIDEOINFO;
	}

	if (VideoInfo->hw_available)
	{
		prv_Core->GetLogFile()->WriteText("Init: Hardware Surface");
		prv_VideoFlags |= SDL_HWSURFACE;
	} else {
		prv_Core->GetLogFile()->WriteText("Init: Software Surface");
		prv_VideoFlags |= SDL_SWSURFACE;
	}

    if (VideoInfo->blit_hw)
	{
		prv_Core->GetLogFile()->WriteText("Init: Hardware Blitting");
		prv_VideoFlags |= SDL_HWACCEL;
	} else {
		prv_Core->GetLogFile()->WriteText("Warn: No Hardware Blitting");
	}

    SDLSurface = SDL_SetVideoMode(Width, Height, ColorDepth, prv_VideoFlags);
    if (!SDLSurface)
	{
		prv_Core->GetLogFile()->WriteText("Error: Set video mode");
		return SDL_ERR_NOSURFACE;
	} else {
		prv_Core->GetLogFile()->WriteText("Init: Set video mode to %ix%ix%i", Width, Height, ColorDepth);
		if(ColorDepth != SDLSurface->format->BitsPerPixel)
			 prv_Core->GetLogFile()->WriteText("Warning: Using %i bpp instead of \
			 %i bpp to avoid shadow surfaces in windowed mode.",
			 SDLSurface->format->BitsPerPixel, ColorDepth);
	}

	//----OpenGL----
    glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	
	char* GLInfo = (char*)glGetString(GL_VENDOR);
	prv_Core->GetLogFile()->WriteText("Info: OpenGL Vendor %s", GLInfo);
	GLInfo = (char*)glGetString(GL_VERSION);
	prv_Core->GetLogFile()->WriteText("Info: OpenGL Version %s", GLInfo);
	GLInfo = (char*)glGetString(GL_EXTENSIONS);
	prv_Core->GetLogFile()->WriteText("Info: OpenGL Extensions %s", GLInfo);

	this->ResizeWindow(Width, Height);

	prv_IsRunning 	= true;
	prv_FPSTime		= SDL_GetTicks();

	prv_Core->GetLogFile()->WriteText("Init: <-- GraphicDevice");
    return OK;
}

//-----------------------------------------------------------------------------
// Name:		BeginRender
// Description:	Begin render process
//-----------------------------------------------------------------------------
void CGraphicDevice::BeginRender(void)
{
	prv_DeltaTime = SDL_GetTicks() - prv_FrameTime;
	prv_FrameTime = SDL_GetTicks();
		
    while(SDL_PollEvent(&prv_SDLEvent))
    {
        switch (prv_SDLEvent.type)
        {
            case SDL_QUIT:
			{
                prv_IsRunning = false;
			} break;
			case SDL_VIDEORESIZE:
			{
				ResizeWindow(prv_SDLEvent.resize.w, prv_SDLEvent.resize.h);
			} break;
			
			default:
				break;
		}
    }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-----------------------------------------------------------------------------
// Name:		EndRender
// Description: End render process
//-----------------------------------------------------------------------------
void CGraphicDevice::EndRender(void)
{
	prv_Core->GetImageLoader()->SetTexture("");

	glFlush();
	glPopMatrix();
	SDL_GL_SwapBuffers();
	
	if((SDL_GetTicks() - prv_FPSTime) >= 1000)
	{
		prv_FPSTime	= SDL_GetTicks();
		prv_CurrentFPS = prv_FPSCount;
		prv_FPSCount = 0;
	}
	prv_FPSCount++;
}

//-----------------------------------------------------------------------------
// Name:		ResizeWindow
// Description: Resize the window and update video mode
//-----------------------------------------------------------------------------
void CGraphicDevice::ResizeWindow(int Width, int Height)
{
    if(0 == Height)
		Height = 1;

    if(0 == Width)
		Width = 1;
		
	prv_WindowHeight	= Height;
	prv_WindowWidth		= Width;
	
	SDL_SetVideoMode(Width, Height, prv_ColorDepth, prv_VideoFlags);
	glEnable(GL_TEXTURE_2D);
	
	if(!prv_IsOrtho)
		SetPerspectiveView();
	else
		SetOrthoView();
}

//-----------------------------------------------------------------------------
// Name:		SetWindowText
// Description: Set the window text
//-----------------------------------------------------------------------------
void CGraphicDevice::SetWindowText(char* Title)
{
	SDL_WM_SetCaption(Title, NULL);
}

//-----------------------------------------------------------------------------
// Name:		SetOrthoView
// Description: Set "2D Mode"
//-----------------------------------------------------------------------------
void CGraphicDevice::SetOrthoView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, prv_WindowWidth, prv_WindowHeight, 0);
	prv_IsOrtho = true;

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
}

//-----------------------------------------------------------------------------
// Name:		SetPerspectiveView
// Description: Set "3D Mode"
//-----------------------------------------------------------------------------
void CGraphicDevice::SetPerspectiveView()
{
	GLfloat Ratio = (GLfloat)prv_WindowWidth / (GLfloat)prv_WindowHeight;
	glViewport(0, 0, (GLsizei)prv_WindowWidth, (GLsizei)prv_WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, Ratio, 0.1f, 4096.0f);
	prv_IsOrtho = false;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

unsigned long CGraphicDevice::GetTicks()
{
	return SDL_GetTicks();
}

bool CGraphicDevice::IsOrtho()
{
	return prv_IsOrtho;
}

void CGraphicDevice::GetWindowSize(int& Width, int& Height)
{
	Height = prv_WindowHeight;
	Width = prv_WindowWidth;
}

int CGraphicDevice::GetFPS()
{
	return prv_CurrentFPS;
}

unsigned long CGraphicDevice::GetDeltaTime()
{
	return prv_DeltaTime;
}

bool CGraphicDevice::IsRunning()
{
	return prv_IsRunning;
}

void CGraphicDevice::Quit()
{
	prv_IsRunning = false;
}

bool CGraphicDevice::ExtSupported(string Ext)
{
    char *p;
    char *End;
    int Len = Ext.length();   
        
    p = (char*)glGetString(GL_EXTENSIONS);
    if (NULL == p)
	{
        return false;
    }

    End = p + strlen(p);   

    while (p < End)
	{
        int n = strcspn(p, " ");
        if ((Len == n) && (strncmp(Ext.c_str(), p, n) == 0))
		{
            return true;
        }
        p += (n + 1);
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name:		GetSDLEvent
// Description:	Get SDL_Event
//-----------------------------------------------------------------------------
SDL_Event* CGraphicDevice::GetSDLEvent()
{
	return &prv_SDLEvent;
}

//-----------------------------------------------------------------------------
// Name:		Destructor
// Description:	Delete class if exist
//-----------------------------------------------------------------------------
CGraphicDevice::~CGraphicDevice()
{
	prv_Core->GetLogFile()->WriteText("Uninit: --> GraphicDevice");
	SDL_Quit();
	prv_Core->GetLogFile()->WriteText("Uninit: <-- GraphicDevice");
}

};
