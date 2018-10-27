//-----------------------------------------------------------------------------
//  SceneNodeModel.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - {SHORT DESCRIPTION} -
//-----------------------------------------------------------------------------

#ifndef __LF3DSCENENODEMODELH__
#define __LF3DSCENENODEMODELH__

namespace LF3D
{

	class CSceneNodeModel: public ISceneNode
	{
		private:
			CCore		*prv_Core;
			string		prv_Model;
			Vector3		prv_Position;
			Vector3 	prv_Rotation;
			bool		prv_IsVisible;
			BoundingBox prv_BoundingBox;
		
		public:
					CSceneNodeModel(CCore* Core, string Model, Vector3 Pos,
									Vector3 Rot);
					~CSceneNodeModel();
			void 					SetPosition(Vector3 Pos);
			void 					SetRotation(Vector3 Rot);	
			void					SetVisible(bool Visible);
			BoundingBox				GetBoundingBox();	
			void 					Update();
			void 					Render();
			void 					RenderNode();
			void 					DeleteNodes();
			vector<ISceneNode*>& 	GetNodes();
	};

};

#endif
