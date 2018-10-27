//-----------------------------------------------------------------------------
//  ModelLoader.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Load 3DS Files -
//-----------------------------------------------------------------------------

#ifndef __LF3DMODELLOADER__
#define __LF3DMODELLOADER__

namespace LF3D
{

	struct OBJ_VERTEX
	{
		float x, y, z;
	};

	struct OBJ_NORMAL
	{
		float x, y, z;
	};

	struct OBJ_TEXTURECOORD
	{
		float u, v;
	};
	
	struct OBJ_FACE
	{
		unsigned short VertexIndex[3];
		unsigned short TextureCoordIndex[3];
		unsigned short NormalIndex[3];
	};
	
	struct OBJ_OBJECT
	{
		char					Name[20];
		vector<OBJ_VERTEX> 		Vertices;
		vector<OBJ_FACE> 		Faces;
		vector<OBJ_NORMAL> 		Normals;
		vector<OBJ_TEXTURECOORD>TextureCoords;
		char					MaterialName[20];
	};
	
	struct OBJ_MATERIAL
	{
		char	Name[20];
		char	TextureName[512];
	};

	struct OBJ_MODEL
	{
		vector<OBJ_OBJECT> 		Objects;
		vector<OBJ_MATERIAL>	Materials;
	};

	class CModelLoader
	{
		private:
			//! Pointer to CoreDevice
			CCore					*prv_Core;
			Vector3					prv_Position;
			Vector3					prv_Rotation;
			map<string, OBJ_MODEL>	prv_Models;
			
		public:
						CModelLoader(CCore *Core);
						~CModelLoader();
			RESULT		LoadModel(string FileName);
			RESULT		LoadMaterial(string FileName);
			void		FreeModel(string FileName);
			void		RenderModel(string FileName);
			void		SetPosition(float x, float y, float z);
			void		SetRotation(float x, float y, float z);
			BoundingBox	GetBoundingBox(string FileName);
	};

};

#endif
