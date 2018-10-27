//-----------------------------------------------------------------------------
//  SceneNode.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - {SHORT DESCRIPTION} -
//-----------------------------------------------------------------------------

#ifndef __LF3DSCENENODEH__
#define __LF3DSCENENODEH__

namespace LF3D
{

	class ISceneNode
	{
		private:
			CCore*				prv_Core;
		
		protected:
			vector<ISceneNode*>	prv_Nodes;

		public:
						ISceneNode(CCore* Core);
			ISceneNode* AddModelNode(string Model, Vector3 Pos, Vector3 Rot);
			ISceneNode* AddOctreeNode();
			
			virtual void		SetVisible(bool Visible);
			virtual void 		SetPosition(int x, int y, int z);
			virtual void 		SetRotation(int x, int y, int z);
			virtual BoundingBox	GetBoundingBox();

			virtual void 		Update() = 0;
			virtual void 		Render() = 0;
			virtual void 		RenderNode() = 0;
			virtual void 		DeleteNodes() = 0;
			virtual vector<ISceneNode*>& GetNodes() = 0;
	};

};

#endif
