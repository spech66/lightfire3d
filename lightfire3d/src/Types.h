//-----------------------------------------------------------------------------
//  Types.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - structs and typedefs needed by the engine -
//-----------------------------------------------------------------------------

#ifndef __LF3DTYPESH__
#define __LF3DTYPESH__

namespace LF3D
{

typedef unsigned char byte;
typedef unsigned short word;

//-----------------------------------------------------------------------------
// Lightfire3D Return Values
//-----------------------------------------------------------------------------
#if defined(LF3DPLATFORM_WINDOWS)
	#undef ERROR
#endif 

enum RESULT
{
	ERROR=0,
	OK,
	SDL_ERR_INIT,
	SDL_ERR_VIDEOINFO,
	SDL_ERR_NOSURFACE,
	ERR_FILENOTFOUND,
	ERR_INVALIDTEXTUREFORMAT,
	NET_NEWCONNECTION,
	NET_MESSAGE,
	NET_CLOSE,
};

//-----------------------------------------------------------------------------
// 3D Vector
//-----------------------------------------------------------------------------
typedef struct Vector3
{
	public:
	float x, y, z;

	Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector3 operator+(Vector3 Value)
	{
		return Vector3(Value.x + x, Value.y + y, Value.z + z);
	}

	Vector3 operator-(Vector3 Value)
	{
        	return Vector3(x - Value.x, y - Value.y, z - Value.z);
	}

	Vector3 operator*(float Value)
	{
        	return Vector3(x * Value, y * Value, z * Value);
	}

	Vector3 operator/(float Value)
	{
        	return Vector3(x / Value, y / Value, z / Value);
	}
} Vector3;

//-----------------------------------------------------------------------------
// BoundingBox
//-----------------------------------------------------------------------------
typedef struct BoundingBox
{
	Vector3 Min;
	Vector3 Max;
	BoundingBox()
	{
		Min = Vector3(1000000, 1000000, 1000000);
		Max = Vector3(-1000000, -1000000, -1000000);
	}
} BoundingBox;

//-----------------------------------------------------------------------------
// TGA File Format Structure
//-----------------------------------------------------------------------------
typedef struct
{
	GLubyte	*imageData;
	GLuint	bpp;
	GLuint	width;
	GLuint	height;
} TGAFILE;

};

#endif
