//-----------------------------------------------------------------------------
// Copyright (C) 2003-2004 Sebastian Pech
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Main.h
//  http://www.spech.de
//  - Main File -
//-----------------------------------------------------------------------------

#ifndef __LF3DMAINH__
#define __LF3DMAINH__

//-----------------------------------------------------------------------------
// OS & API Defines and Includes
//-----------------------------------------------------------------------------
#if defined(__linux__) || defined (__FreeBSD__)
	#define LF3DPLATFORM_LINUX
	#include <stdarg.h>
	#include <dlfcn.h>
	#include <sys/stat.h>
	#include <X11/Xlib.h>
	#include <dirent.h>
	#include <GL/glx.h>
	
	#include <sys/types.h>	//Network
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>		//File descriptor manipulation
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined (__GNUWIN32__)
	#define LF3DPLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <SDL_syswm.h>
	#include <winsock2.h>	//Network
#else
	#error WIN32 & Linux only!
#endif

#include <GL/gl.h>	//OpenGL
#include <GL/glu.h> //OpenGL Utilities
#include <SDL.h>	//SDL
#include <audiere.h>//Audiere
#include <math.h>	//System
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

namespace LF3D
{
	#if defined(LF3DPLATFORM_LINUX)
		#define DWORD					unsigned long
		#define _vsnprintf				vsnprintf
		#define ZeroMemory(Dest, Len)	memset((Dest),0,(Len))
	#elif defined(LF3DPLATFORM_WINDOWS)
	#endif

	//-----------------------------------------------------------------------------
	// Defines
	//-----------------------------------------------------------------------------
	#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
	#define RAD_TO_DEG				57.29577951308232286465f
	#define PI						3.141592653589793238463f
};

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
namespace LF3D
{
	class CCore;
}

#include "Types.h"
#include "Helper.h"
#include "LogFile.h"
#include "FontLoader.h"
#include "ImageLoader.h"
#include "ModelLoader.h"
#include "TerrainLoader.h"
#include "GraphicDevice.h"
#include "InputDevice.h"
#include "AudioDevice.h"
#include "CameraDevice.h"
#include "SceneNode.h"
#include "SceneNodeModel.h"
#include "SceneNodeOctree.h"
#include "SceneGraph.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

namespace LF3D
{
	
	class CCore
	{
		private:
			CLogFile 			*prv_EngineLogFile;
			CGraphicDevice		*prv_GraphicDevice;
			CInputDevice		*prv_InputDevice;
			CAudioDevice		*prv_AudioDevice;
			CCameraDevice		*prv_CameraDevice;
			CFontLoader			*prv_FontLoader;
			CImageLoader		*prv_ImageLoader;
			CModelLoader		*prv_ModelLoader;
			CTerrainLoader		*prv_TerrainLoader;
			CSceneGraph			*prv_SceneGraph;
			CNetworkClient		*prv_NetworkClient;
			CNetworkServer		*prv_NetworkServer;
			
		public:
			CCore();
			CLogFile*			GetLogFile();
			CGraphicDevice*		GetGraphicDevice();
			CInputDevice*		GetInputDevice();
			CAudioDevice*		GetAudioDevice();
			CCameraDevice*		GetCameraDevice();
			CFontLoader*		GetFontLoader();
			CImageLoader*		GetImageLoader();
			CModelLoader*		GetModelLoader();
			CTerrainLoader*		GetTerrainLoader();
			CSceneGraph*		GetSceneGraph();
			CNetworkClient*		GetNetworkClient();
			CNetworkServer*		GetNetworkServer();
			~CCore();
	};

};

#endif
