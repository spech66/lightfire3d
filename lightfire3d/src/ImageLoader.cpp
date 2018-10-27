//-----------------------------------------------------------------------------
//  ImageLoader.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Loading tga Files -
//-----------------------------------------------------------------------------

// IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !
// The Image Loader LoadImage Function is currently based on NeHe`s Tutorial.
// The function will be replaced with own code as soon as possible. For now
// leave this message as it is!
// -> http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=24
// IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !! IMPORTANT !

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

CImageLoader::CImageLoader(CCore *Core)
{
	prv_Core = Core;
}

CImageLoader::~CImageLoader()
{
	for(map<string, GLuint>::iterator i = prv_ImageHandles.begin(); i != prv_ImageHandles.end(); i++)
	{
		glDeleteTextures(1, &(*i).second);
		prv_ImageHandles.erase(i);
	}

	for(map<string, TGAFILE*>::iterator j = prv_TgaHandles.begin(); j != prv_TgaHandles.end(); j++)
	{
		if(j->second != NULL)
		{
			if(NULL != (*j).second->imageData)
			{
					free((*j).second->imageData);
					(*j).second->imageData = NULL;
			}
			delete j->second;
			j->second = NULL;
		}
		prv_TgaHandles.erase(j);
	}
		
	prv_Core->GetLogFile()->WriteText("Uninit: Free Images");
}

RESULT CImageLoader::LoadImage(string FileName)
{
	if(prv_TgaHandles.count(FileName) != 0)
		return OK;

	TGAFILE		*texture;
	texture		= new TGAFILE;
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *File = fopen(FileName.c_str(), "rb");

	if(File == NULL)
	{
		prv_Core->GetLogFile()->WriteText("Error: LoadImage(%s) File not found", FileName.c_str());
		return ERR_INVALIDTEXTUREFORMAT;
	}

	if(fread(TGAcompare,1,sizeof(TGAcompare),File)!=sizeof(TGAcompare) ||
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||
		fread(header,1,sizeof(header),File)!=sizeof(header))
	{
		fclose(File);
		prv_Core->GetLogFile()->WriteText("Error: LoadImage(%s) Inavlid Texture Format", FileName.c_str());
		return ERR_INVALIDTEXTUREFORMAT;
	}

	texture->width  = header[1] * 256 + header[0];
	texture->height = header[3] * 256 + header[2];

 	if(	texture->width <=0 || texture->height <=0 || (header[4]!=24 && header[4]!=32))
	{
		fclose(File);
		prv_Core->GetLogFile()->WriteText("Error: LoadImage(%s) Inavlid Texture Format", FileName.c_str());
		return ERR_INVALIDTEXTUREFORMAT;
	}

	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(texture->imageData == NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, File)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData != NULL)
			free(texture->imageData);

		fclose(File);
		prv_Core->GetLogFile()->WriteText("Error: LoadImage(%s) Inavlid Texture Format", FileName.c_str());
		return ERR_INVALIDTEXTUREFORMAT;
	}

	for(GLuint i=0; i < int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (File);

	if(texture[0].bpp == 24)
		type = GL_RGB;

	GLuint TextureHandle;
    glPushAttrib(GL_TEXTURE_BIT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &TextureHandle);
    glBindTexture(GL_TEXTURE_2D, TextureHandle);
    gluBuild2DMipmaps(GL_TEXTURE_2D, type, texture->width,
		texture->height, type, GL_UNSIGNED_BYTE, texture->imageData);
    glPopAttrib();
	prv_ImageHandles[FileName] = TextureHandle;
	prv_TgaHandles[FileName] = texture;
		
	prv_Core->GetLogFile()->WriteText("Init: LoadImage(%s)", FileName.c_str());

	return OK;

}

void CImageLoader::FreeImage(string FileName)
{
	if(prv_TgaHandles.count(FileName) == 0)
		return;
	
	glDeleteTextures(1, &prv_ImageHandles[FileName]);
	
	if(prv_TgaHandles[FileName] != NULL)
	{
		if(prv_TgaHandles[FileName]->imageData != NULL)
			free(prv_TgaHandles[FileName]->imageData);

		delete prv_TgaHandles[FileName];
		prv_TgaHandles[FileName] = NULL;
	}

	prv_TgaHandles.erase(FileName);
	prv_ImageHandles.erase(FileName);
	
	prv_Core->GetLogFile()->WriteText("Uninit: FreeImage(%s)", FileName.c_str());
}

void CImageLoader::SetTexture(string FileName)
{
	if(FileName == "0" || FileName == "")
	{
		prv_LastTexture = "0";
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	
	if(prv_TgaHandles.count(FileName) == 0)
		return;
	if(prv_LastTexture == FileName)	// Avoid unnecessary calls
		return;
	
	prv_LastTexture = FileName;
	glBindTexture(GL_TEXTURE_2D, prv_ImageHandles[FileName]);
}

TGAFILE* CImageLoader::GetTexture(string FileName)
{
	if(prv_TgaHandles.count(FileName) == 0)
		return NULL;
	
	return prv_TgaHandles[FileName];
}

};
