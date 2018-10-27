//-----------------------------------------------------------------------------
//  ImageLoader.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Loading tga Files -
//-----------------------------------------------------------------------------

#ifndef __LF3DIMAGELOADERH__
#define __LF3DIMAGELOADERH__

namespace LF3D
{

	class CImageLoader
	{
		private:
			//! Pointer to CoreDevice
			CCore					*prv_Core;
			//! A map that contains all the texture IDs
			map<string, GLuint>		prv_ImageHandles;
			//! A map that contains all the image data
			map<string, TGAFILE*>	prv_TgaHandles;
			//! Contains the last used Texture to optimize SetTexture Calls
			string					prv_LastTexture;

		public:
			//! Constructor
					CImageLoader(CCore *Core);
			//! Destructor 
					~CImageLoader();
			//! Loads an Image from a TGA file
			/*!
				Opens a TGA File, reads the data and stores it as raw image data and an OpenGL texture.
				Also creates mipmaps for the texture.
				\param FileName The file the image should be loaded from. This will also be the image's name in the database.
				\return Returns an error code. Can be OK or ERR_INVALIDTEXTUREFORMAT
				\sa FreeImage(), SetTexture(), GetTexture()
			*/
			RESULT	LoadImage(string FileName);
			//! Unloads an loaded image
			/*!
				Looks for the specified image in the Storage and unloads it if it exists.
				\param FileName The filename that was specified while loading the Image.
				\sa LoadImage()
			*/
			void		FreeImage(string FileName);
			//! Binds a loaded Texture 
			/*!
				Looks for the specified texture in the storage and binds it if it exists.
				A bound texture will be used as the Texture all objects rendered from now on. 
				\param FileName The filename that was specified while loading the Image.
			*/
			void		SetTexture(string FileName);
			//! Returns a pointer to an Image
			/*!
				Looks for the specified image in the story and returns a pointer to it if it exists
				\param FileName The filename that was specified while loading the Image.
				\return A pointer to the image data or NULL if the image doesn't exist.
			*/
			TGAFILE* 	GetTexture(string FileName);
	};

};

#endif
