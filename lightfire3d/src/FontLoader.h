//-----------------------------------------------------------------------------
//  FontLoader.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Loading tga Files -
//-----------------------------------------------------------------------------

#ifndef __LF3DFONTLOADERH__
#define __LF3DFONTLOADERH__

namespace LF3D
{

	class CFontLoader
	{
		private:
			//! Pointer to CoreDevice
			CCore				*prv_Core;
			map<string, GLuint>	prv_FontHandles;
			Vector3				prv_Position;
			int					prv_ColorR, prv_ColorG, prv_ColorB;

		public:
						CFontLoader(CCore *Core);
						~CFontLoader();
			RESULT		LoadFont(string Name);
			RESULT		LoadFont(string Name, int Size);
			void		FreeFont(string Name);
			void		SetPosition(int X, int Y);
			void		SetColor(int R, int G, int B);
			void		PrintText(string Name, string Text);
	};

};

#endif
