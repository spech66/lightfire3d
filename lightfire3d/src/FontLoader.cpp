//-----------------------------------------------------------------------------
//  FontLoader.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Loading fonts -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CFontLoader
// Description:	Constructor
//-----------------------------------------------------------------------------
CFontLoader::CFontLoader(CCore *Core)
{
	prv_Core = Core;
	prv_Position = Vector3(0, 0, 0);
	prv_ColorR = 255;
	prv_ColorG = 255;
	prv_ColorB = 255;
}

//-----------------------------------------------------------------------------
// Name:		~CFontLoader
// Description:	Destructor
//-----------------------------------------------------------------------------
CFontLoader::~CFontLoader()
{
	for(map<string, GLuint>::iterator i = prv_FontHandles.begin(); i != prv_FontHandles.end(); i++)
	{
		if((*i).second)
			glDeleteLists(1, (*i).second);
		prv_FontHandles.erase(i);
	}
	prv_Core->GetLogFile()->WriteText("Uninit: Free Fonts");
}

//-----------------------------------------------------------------------------
// Name:		LoadFont
// Description:	Creates a font (Uses fontsize 12 for Windows)
//-----------------------------------------------------------------------------
RESULT CFontLoader::LoadFont(string Name)
{
	return LoadFont(Name, 12);
}

//-----------------------------------------------------------------------------
// Name:		LoadFont
// Description:	Size is used for Windows only
//-----------------------------------------------------------------------------
RESULT CFontLoader::LoadFont(string Name, int Size)
{
	if(prv_FontHandles[Name])
		return OK;

	GLuint	FontList = glGenLists(96);

	#if defined(LF3DPLATFORM_LINUX)
		Display *Dpy;
		XFontStruct *FontInfo = NULL;

		Dpy = XOpenDisplay(NULL);
		FontInfo = XLoadQueryFont(Dpy, Name.c_str());

		if(FontInfo == NULL)
		{
			prv_Core->GetLogFile()->WriteText("Error: LoadFont(%s)", Name.c_str());
			XCloseDisplay(Dpy);
			return ERROR;
		}

		glXUseXFont(FontInfo->fid, 32, 96, FontList);

		XFreeFont(Dpy, FontInfo);
		XCloseDisplay(Dpy);

	#elif defined(LF3DPLATFORM_WINDOWS)
		HFONT	font;
		SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
		SDL_GetWMInfo(&wmi);
		HDC hDC = GetDC(wmi.window);

		int FontSize = Size;
		string FontName = Name;

		font = CreateFont(-FontSize, 0, 0, 0, FW_BOLD, false, false, false,
						ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH,
						FontName.c_str());
						
		if(NULL == font)
		{
			prv_Core->GetLogFile()->WriteText("Error: LoadFont(%s)", Name.c_str());
			return ERROR;
		}

		SelectObject(hDC, font);
		wglUseFontBitmaps(hDC, 32, 96, FontList);
		DeleteObject(font);
		
	#endif
	
	prv_FontHandles[Name] = FontList;
	prv_Core->GetLogFile()->WriteText("Init: LoadFont(%s)", Name.c_str());

	return OK;
}

//-----------------------------------------------------------------------------
// Name:		FreeFont
// Description:	Delete font
//-----------------------------------------------------------------------------
void CFontLoader::FreeFont(string Name)
{
	if(!prv_FontHandles[Name])
		return;

	glDeleteLists(1, prv_FontHandles[Name]);
	prv_FontHandles.erase(Name);
	prv_Core->GetLogFile()->WriteText("Uninit: FreeFont(%s)", Name.c_str());
}

//-----------------------------------------------------------------------------
// Name:		SetPosition
// Description:	Set font Position
//-----------------------------------------------------------------------------
void CFontLoader::SetPosition(int X, int Y)
{
	prv_Position.x = X;
	prv_Position.y = Y;
}

//-----------------------------------------------------------------------------
// Name:		SetColor
// Description:	Set the font Color
//-----------------------------------------------------------------------------
void CFontLoader::SetColor(int R, int G, int B)
{
	prv_ColorR = R;
	prv_ColorG = G;
	prv_ColorB = B;
}

//-----------------------------------------------------------------------------
// Name:		PrintText
// Description:	Print "Text" with "Name" font
//-----------------------------------------------------------------------------
void CFontLoader::PrintText(string Name, string Text)
{
	if(!prv_FontHandles[Name])
		return;
	
	if("" == Text.c_str())
		return;

	glColor3ub(prv_ColorR, prv_ColorG, prv_ColorB);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2f(prv_Position.x, prv_Position.y);
	glPushAttrib(GL_LIST_BIT);
	glListBase(prv_FontHandles[Name] - 32);
	glCallLists(Text.length(), GL_UNSIGNED_BYTE, Text.c_str());
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 255);
}

};
