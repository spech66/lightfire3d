//-----------------------------------------------------------------------------
//  SceneNodeOctree.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Octree and Nodes -
//-----------------------------------------------------------------------------

#ifndef __LF3DSCENENODEOCTREEH__
#define __LF3DSCENENODEOCTREEH__

namespace LF3D
{
	class COctreeNode
	{
		private:
			vector<COctreeNode*>prv_ChildNodes;
			vector<ISceneNode*>	prv_SceneNodes;
			
		public:
			void	Create(vector<ISceneNode*> SceneNodes, int Depth, int MaxDepth);
			void	Delete();
			void	Render();
	};

	class CSceneNodeOctree: public ISceneNode
	{
		private:
			CCore			*prv_Core;
			COctreeNode		RootNode;
			int				MaxDepth;
			
			void			BuildTree();
			
		public:
					CSceneNodeOctree(CCore* Core);
					~CSceneNodeOctree();
			void 					Update();
			void 					Render();
			void 					RenderNode();
			void 					DeleteNodes();
			vector<ISceneNode*>& 	GetNodes();
	};

};

#endif
