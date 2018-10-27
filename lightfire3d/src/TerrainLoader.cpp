//-----------------------------------------------------------------------------
//  TerrrainLoader.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Load Small Terrains -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

CTerrainLoader::CTerrainLoader(CCore *Core)
{
	prv_Core = Core;
	prv_Position = Vector3(0.0f, 0.0f, 0.0f);
	prv_Rotation = Vector3(0.0f, 0.0f, 0.0f);
}

CTerrainLoader::~CTerrainLoader()
{
	for(map<string, unsigned char*>::iterator i = prv_Terrains.begin(); i != prv_Terrains.end(); i++)
	{
		SAFE_DELETE((*i).second);
		prv_Terrains.erase(i);
	}
}

//-----------------------------------------------------------------------------
// Name:		NAME
// Description:	DESC
//-----------------------------------------------------------------------------
RESULT CTerrainLoader::LoadTerrain(string FileName)
{
	if(prv_Terrains[FileName])
		return OK;

	unsigned char* TerrainData = new unsigned char[128*128];
	
	FILE *TerrainFile = NULL;
	if(NULL == (TerrainFile = fopen(FileName.c_str(), "rb"))) 
    {
        prv_Core->GetLogFile()->WriteText("Init: LoadTerrain(%s) file not found", FileName.c_str());
        return ERR_FILENOTFOUND;
    }

	fread(TerrainData, 1, 128*128, TerrainFile);

    if(ferror(TerrainFile))
    {
        prv_Core->GetLogFile()->WriteText("Error: LoadTerrain(%s) could not get the data", FileName.c_str());
		return ERROR;
    }
	fclose(TerrainFile);
	
	prv_Terrains[FileName] = TerrainData;
	
	prv_Core->GetLogFile()->WriteText("Init: LoadTerrain(%s)", FileName.c_str());

	return OK;
}

void CTerrainLoader::RenderTerrain(string FileName)
{
	unsigned char* TerrainData = prv_Terrains[FileName];
	bool SwitchSides = false;
	
	float ScaleHeight = 0.25f;
	float ScaleXZ = 2.0f;
	
	glPushMatrix();
	glTranslatef(prv_Position.x, prv_Position.y, prv_Position.z);
	glRotatef(prv_Rotation.x, 1, 0, 0);
	glRotatef(prv_Rotation.y, 0, 1, 0);
	glRotatef(prv_Rotation.z, 0, 0, 1);
	
	glBegin(GL_TRIANGLE_STRIP);
	for(int x = 0; x <= 128; x++)
	{
		if(SwitchSides)
        { 
			for(int z = 128; z >= 0; z--)
			{	
				glTexCoord2f((float)x/128.0f, -(float)z/128.0f);
				glVertex3f(x*ScaleXZ, (float)TerrainData[x + (z*128)] * ScaleHeight, z*ScaleXZ);
				
				glTexCoord2f((float)(x+1)/128.0f, -(float)z/128.0f);
				glVertex3f((x+1)*ScaleXZ, (float)TerrainData[x+1 + (z*128)] * ScaleHeight, z*ScaleXZ);
			}
		} else {
			for(int z = 0; z <= 128; z++)
			{
				glTexCoord2f((float)(x+1)/128.0f, -(float)z/128.0f);
				glVertex3f((x+1)*ScaleXZ, (float)TerrainData[x+1 + (z*128)] * ScaleHeight, z*ScaleXZ);
				
				glTexCoord2f((float)x/128.0f, -(float)z/128.0f);
				glVertex3f(x*ScaleXZ, (float)TerrainData[x + (z*128)] * ScaleHeight, z*ScaleXZ);
			}
		}
		SwitchSides = !SwitchSides;
	}
	glEnd();
	
	glPopMatrix();
}

void CTerrainLoader::SetPosition(float x, float y, float z)
{
	prv_Position.x = x;
	prv_Position.y = y;
	prv_Position.z = z;
}

void CTerrainLoader::SetRotation(float x, float y, float z)
{
	prv_Rotation.x = x;
	prv_Rotation.y = y;
	prv_Rotation.z = z;
}

};
