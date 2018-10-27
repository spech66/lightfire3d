//-----------------------------------------------------------------------------
//  SceneGraph.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Manage scene nodes -
//-----------------------------------------------------------------------------

#ifndef __LF3DSCENEGRAPHH__
#define __LF3DSCENEGRAPHH__

namespace LF3D
{

	class CSceneGraph
	{
		private:
			//! Pointer to CoreDevice
			CCore				*prv_Core;
			vector<ISceneNode*>	prv_Nodes;
			
		public:
						CSceneGraph(CCore *Core);
						~CSceneGraph();
			ISceneNode* AddModelNode(string Model, Vector3 Pos, Vector3 Rot);
			ISceneNode* AddOctreeNode();
			
			void 		Render();
			void 		Update();
	};

};

#endif
