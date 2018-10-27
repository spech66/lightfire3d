//-----------------------------------------------------------------------------
//  TerrrainLoader.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Load Small Terrains -
//-----------------------------------------------------------------------------

#ifndef __LF3DFILE__
#define __LF3DFILE__

namespace LF3D
{

	class CTerrainLoader
	{
		private:
			//! Pointer to CoreDevice
			CCore						*prv_Core;
			Vector3						prv_Position;
			Vector3						prv_Rotation;
			map<string, unsigned char*> prv_Terrains;
			
		public:
					CTerrainLoader(CCore *Core);
					~CTerrainLoader();
			RESULT 	LoadTerrain(string FileName);
			void	RenderTerrain(string FileName);
			void	SetPosition(float x, float y, float z);
			void	SetRotation(float x, float y, float z);
	};

};

#endif
