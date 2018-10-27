//-----------------------------------------------------------------------------
//  GraphicDevice.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle OpenGL and SDL -
//-----------------------------------------------------------------------------

#ifndef __LF3DGRAPHICDEVICEH__
#define __LF3DGRAPHICDEVICEH__

namespace LF3D
{
	//! Class for basic input/output stuff 
	/*!
		This handles window creation, OpenGL initialization, eventhandling and mouse and keyboard input
	*/
	class CGraphicDevice
	{
		private:
			//! Pointer to CoreDevice
			CCore			*prv_Core;
			//! Used for buffering events
			SDL_Event		prv_SDLEvent;
			//! Contains video flags used by the SDL display surface
			int				prv_VideoFlags;
			//! Contains the main window's size and color depth
			int				prv_WindowHeight, prv_WindowWidth, prv_ColorDepth;
			//! This indicates if orthographic mode is used
			bool			prv_IsOrtho;
			//! Is Running for Game Loop
			bool			prv_IsRunning;
			//! Indicates the current frames per second
			int				prv_CurrentFPS;
			int				prv_FPSCount;
			unsigned long 	prv_FPSTime;
			//! Delta Time calculation
			unsigned long 	prv_FrameTime;
			unsigned long 	prv_DeltaTime;

		public:
			
			//! Constructor
							CGraphicDevice(CCore *Core);
			//! Destructor
							~CGraphicDevice();
			//! Initializes GraphicDevice
			/*!
				Spawns a new window if necessary.
				\param bWindowed If this is set to true, windowed mode is used. Otherwise fullscreen mode is used.
				\param Width Contains the desired horizontal resolution
				\param Height Contains the desired vertical resolution
				\param ColorDepth Contains the desired color depth in bits per pixel. Warning: In windowed mode, the actual color depth may differ from this value.
			*/
			RESULT				Init(bool bWindowed, int Width, int Height, int ColorDepth);
			//! Begins a frame
			/*!
				This function makes everything ready for rendering and parses mouse and keyboard events.
				You should call this at the beginning of evey frame, ohterwise things may behave in a strange way.
				\sa EndRender()
			*/
			void				BeginRender(void);
			//! Ends a frame
			/*!
				This ends a frame by switching front and back buffer and clearing the back buffer afterwards.
				\sa BeginRender()
			*/
			void				EndRender(void);
			//! Resizes the main window
			/*!
				SetOrthoView() or SetPerspectiveView() is called afterwards if necesarry
				\param Width Contains the desired horizontal resolution
				\param Height Contains the desired vertical resolution
			*/
			void				ResizeWindow(int Width, int Height);
			//! Changes the window's title
			/*!
				\param Title The windows new title.
			*/
			void				SetWindowText(char* Title);
			//! Changes the view mode to orthographic mode. Uses gluOrtho2d()
			void				SetOrthoView();
			//! Changes the view mode to perspective mode. Uses gluPerspective.
			/*!
				Far clipping plane is set to 4096
			*/
			void				SetPerspectiveView();
			//! Indicates if orthographic mode is used
			/*!
				\return Returns true if orthographic view mode is used, or false if perspective mode is used.
			*/
			bool				IsOrtho();
			//! This does exactly the same as SDL_GetTicks()
			unsigned long		GetTicks();
			//! Returns the window's size
			/*!
				\param Width The window's horizontal size will be stored here.
				\param Height The window's vertical size will be stored here.
			*/
			void				GetWindowSize(int& Width, int& Height);
			//! GetFPS
			/*!
				\return Get frames per second
			*/
			int 				GetFPS();
			//! GetFPS
			/*!
				\return Get delta time
			*/
			unsigned long		GetDeltaTime();
			//! Is running
			/*!
				\return Returns the current status.
			*/
			bool 				IsRunning();
			//! Quit
			/*!
				\return Set running to false
			*/
			void 				Quit();
			//! Checks if a certain OpenGL extension is supported on this machine.
			/*!
				\param Ext Name of the extension
				\return Returns true if the extension is supported
			*/
			bool 				ExtSupported(string Ext);
			//! Get the SDL Event variable
			/*!
				\return Returns pointer to the SDL_Event
			*/
			SDL_Event*			GetSDLEvent();
	};

};

#endif
