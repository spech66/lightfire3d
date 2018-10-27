//-----------------------------------------------------------------------------
//  ModelLoader.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Load 3DS Files -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CModelLoader
// Description:	Constructor
//-----------------------------------------------------------------------------
CModelLoader::CModelLoader(CCore *Core)
{
	prv_Core = Core;
	prv_Position = Vector3(0.0f, 0.0f, 0.0f);
	prv_Rotation = Vector3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
// Name:		~CModelLoader
// Description:	Destructor
//-----------------------------------------------------------------------------
CModelLoader::~CModelLoader()
{
	for(map<string, OBJ_MODEL>::iterator i = prv_Models.begin(); i != prv_Models.end(); i++)
	{
		for(int j = 0; j < (*i).second.Objects.size(); j++)
		{
			(*i).second.Objects[j].Vertices.empty();
			(*i).second.Objects[j].Faces.empty();
			(*i).second.Objects[j].Normals.empty();
			(*i).second.Objects[j].TextureCoords.empty();
		}
		
		(*i).second.Objects.empty();
		(*i).second.Materials.empty();
		prv_Models.erase(i);
	}
		
	prv_Core->GetLogFile()->WriteText("Uninit: Free Models");
}

//-----------------------------------------------------------------------------
// Name:		NAME
// Description:	DESC
//-----------------------------------------------------------------------------
RESULT CModelLoader::LoadModel(string FileName)
{
	if(0 < prv_Models.count(FileName))
		return OK;

	FILE 			*ModelFile;
	unsigned int 	FileLength;
	unsigned short 	ChunkId;
	unsigned int 	ChunkLenght;
	char 			Char;			
	unsigned short 	Qty;
	unsigned short 	FaceFlags;
	int				i;
	OBJ_VERTEX		Vertex;
	OBJ_NORMAL		Normal;
	OBJ_TEXTURECOORD TexCoord;
	OBJ_FACE		Face;
	OBJ_OBJECT		Object;
	OBJ_MATERIAL	Material;
	OBJ_MODEL		Model;
	
	if(NULL == (ModelFile = fopen(FileName.c_str(), "rb")))
	{
		prv_Core->GetLogFile()->WriteText("Error: LoadModel(%s) File not found", FileName.c_str());
		return ERR_FILENOTFOUND;
	}

	fseek(ModelFile, 0, SEEK_END);
	FileLength = ftell(ModelFile);
	rewind(ModelFile);
	while(ftell(ModelFile) < FileLength)
	{
		fread(&ChunkId, 2, 1, ModelFile);
		fread(&ChunkLenght, 4, 1, ModelFile);
		switch (ChunkId)
		{
			case 0x4d4d: //Main
			break;
			
			case 0x3d3d: //Editor
        	break;
			
			case 0x4000: //Object
				Model.Objects.push_back(Object);
				Object.TextureCoords.clear();
				Object.Vertices.clear();
				Object.Faces.clear();
				i=0;
				do
				{
					fread (&Char, 1, 1, ModelFile);
					Object.Name[i] = Char;
					i++;
				}
				while(Char != '\0' && i<20);
			break;
		 
			case 0x4100:
			break;
		 
			case 0x4110: //Vertices
				fread(&Qty, sizeof (unsigned short), 1, ModelFile);
				for (i=0; i < Qty; i++)
				{
					fread (&Vertex.x, sizeof(float), 1, ModelFile);
					fread (&Vertex.y, sizeof(float), 1, ModelFile);
					fread (&Vertex.z, sizeof(float), 1, ModelFile);
					Object.Vertices.push_back(Vertex);
				}
			break;
		 
			case 0x4120: //Faces
				fread (&Qty, sizeof (unsigned short), 1, ModelFile);
				for (i=0; i < Qty; i++)
				{
					fread (&Face.VertexIndex[0], sizeof (unsigned short), 1, ModelFile);
					fread (&Face.VertexIndex[1], sizeof (unsigned short), 1, ModelFile);
					fread (&Face.VertexIndex[2], sizeof (unsigned short), 1, ModelFile);
					fread (&FaceFlags, sizeof (unsigned short), 1, ModelFile);
					Object.Faces.push_back(Face);
				}
			break;
			
			case 0x4130: //mat infos
				i=0;
				do
				{
					fread (&Char, 1, 1, ModelFile);
					Object.MaterialName[i] = Char;
					i++;
				}
				while(Char != '\0' && i < 20);
				fseek(ModelFile, ChunkLenght-i-6, SEEK_CUR);
			break;
		 
			case 0x4140: //texture coordinates
				fread (&Qty, sizeof (unsigned short), 1, ModelFile);
				for (i=0; i<Qty; i++)
				{
					fread (&TexCoord.u, sizeof (float), 1, ModelFile);
					fread (&TexCoord.v, sizeof (float), 1, ModelFile);
					Object.TextureCoords.push_back(TexCoord);
				}
			break;
			
			case 0xAFFF: //material
				Model.Materials.push_back(Material);
			break;
			
			case 0xA000:
				i=0;
				do
				{
					fread (&Char, 1, 1, ModelFile);
					Material.Name[i] = Char;
					i++;
				}
				while(Char != '\0' && i<20);	
			break;
			
			case 0xA200:
			break;

			case 0xA300:
				i=0;
				do
				{
					fread (&Char, 1, 1, ModelFile);
					Material.TextureName[i] = Char;
					i++;
				}
				while(Char != '\0' && i<20);
			break;
		 
			default:
				fseek(ModelFile, ChunkLenght-6, SEEK_CUR);
		} 
		 
	}
	fclose(ModelFile);
	
	Model.Objects.push_back(Object);
	Model.Materials.push_back(Material);
	
	Model.Objects.erase(Model.Objects.begin());
	Model.Materials.erase(Model.Materials.begin());
	
	for(int i = 0; i < Model.Materials.size(); i++)
	{
		int pos = FileName.rfind("/", FileName.length());
		string Temp = FileName.substr(0, pos + 1) + Model.Materials[i].TextureName;
		snprintf(Model.Materials[i].TextureName, 512, "%s", Temp.c_str());
		prv_Core->GetImageLoader()->LoadImage(Model.Materials[i].TextureName);
	}
	
	prv_Core->GetLogFile()->WriteText("Init: LoadModel(%s) with %i Objects and %i Materials",
							FileName.c_str(), Model.Objects.size(), Model.Materials.size());
	
	prv_Models[FileName] = Model;

	return OK;
};

void CModelLoader::RenderModel(string FileName)
{
	if(0 == prv_Models.count(FileName))
		return;

	OBJ_MODEL Model = prv_Models[FileName];
	
	glPushMatrix();
	glTranslatef(prv_Position.x, prv_Position.y, prv_Position.z);
	glRotatef(prv_Rotation.x, 1, 0, 0);
	glRotatef(prv_Rotation.y, 0, 1, 0);
	glRotatef(prv_Rotation.z, 0, 0, 1);
	
	for(int i = 0; i < Model.Objects.size(); i++)
	{
		for(int f = 0; f < Model.Materials.size(); f++)
		{
			if(0 == strncmp(Model.Materials[f].Name, Model.Objects[i].MaterialName, 21))
			{		
				prv_Core->GetImageLoader()->SetTexture(Model.Materials[f].TextureName);
			}
		}
		
		glBegin(GL_TRIANGLES);
		for(int j = 0; j < Model.Objects[i].Faces.size(); j++)
		{
			int	VertexIndex[3];
			VertexIndex[0] = Model.Objects[i].Faces[j].VertexIndex[0];
			VertexIndex[1] = Model.Objects[i].Faces[j].VertexIndex[1];
			VertexIndex[2] = Model.Objects[i].Faces[j].VertexIndex[2];

			glTexCoord2f(Model.Objects[i].TextureCoords[VertexIndex[0]].u,
						Model.Objects[i].TextureCoords[VertexIndex[0]].v);
			glVertex3f(Model.Objects[i].Vertices[VertexIndex[0]].x,
						Model.Objects[i].Vertices[VertexIndex[0]].y,
						Model.Objects[i].Vertices[VertexIndex[0]].z);
						
			glTexCoord2f(Model.Objects[i].TextureCoords[VertexIndex[1]].u,
						Model.Objects[i].TextureCoords[VertexIndex[1]].v);
			glVertex3f(Model.Objects[i].Vertices[VertexIndex[1]].x,
						Model.Objects[i].Vertices[VertexIndex[1]].y,
						Model.Objects[i].Vertices[VertexIndex[1]].z);
						
			glTexCoord2f(Model.Objects[i].TextureCoords[VertexIndex[2]].u,
						Model.Objects[i].TextureCoords[VertexIndex[2]].v);
			glVertex3f(Model.Objects[i].Vertices[VertexIndex[2]].x,
						Model.Objects[i].Vertices[VertexIndex[2]].y,
						Model.Objects[i].Vertices[VertexIndex[2]].z);
		}
		glEnd();
	}
	
	glPopMatrix();
}

void CModelLoader::SetPosition(float x, float y, float z)
{
	prv_Position.x = x;
	prv_Position.y = y;
	prv_Position.z = z;
}

void CModelLoader::SetRotation(float x, float y, float z)
{
	prv_Rotation.x = x;
	prv_Rotation.y = y;
	prv_Rotation.z = z;
}

BoundingBox	CModelLoader::GetBoundingBox(string FileName)
{
	BoundingBox TempBox;
	
	if(0 == prv_Models.count(FileName))
		return TempBox;
		
	int i, h;
	for(i = 0; i < prv_Models[FileName].Objects.size(); i++)
	{
		for(h = 0; h < prv_Models[FileName].Objects[i].Vertices.size(); h++)
		{
			OBJ_VERTEX TempVert = prv_Models[FileName].Objects[i].Vertices[h];

			//TempVert.x = TempVert.x * cos(prv_Rotation.x * PI / 180.0f) -
			//				TempVert.y * sin(prv_Rotation.x * PI / 180.0f);
			//TempVert.y = TempVert.x * sin(prv_Rotation.y * PI / 180.0f) +
			//				TempVert.y * cos(prv_Rotation.y * PI / 180.0f);
			//TempVert.z = TempVert.z * sin(prv_Rotation.z * PI / 180.0f) +
			//				TempVert.z * cos(prv_Rotation.z * PI / 180.0f);
			
			if(TempVert.x < TempBox.Min.x) TempBox.Min.x = TempVert.x;
			if(TempVert.y < TempBox.Min.y) TempBox.Min.y = TempVert.y;
			if(TempVert.z < TempBox.Min.z) TempBox.Min.z = TempVert.z;
			
			if(TempVert.x > TempBox.Max.x) TempBox.Max.x = TempVert.x;
			if(TempVert.y > TempBox.Max.y) TempBox.Max.y = TempVert.y;
			if(TempVert.z > TempBox.Max.z) TempBox.Max.z = TempVert.z;
		}
	}

	TempBox.Min.x += prv_Position.x;
	TempBox.Max.x += prv_Position.x;
	TempBox.Min.y += prv_Position.y;
	TempBox.Max.y += prv_Position.y;
	TempBox.Min.z += prv_Position.z;
	TempBox.Max.z += prv_Position.z;
	
	return TempBox;
}

};
