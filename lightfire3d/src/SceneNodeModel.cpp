//-----------------------------------------------------------------------------
//  SceneNodeModel.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - {SHORT DESCRIPTION} -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CSceneNodeModel
// Description:	Contructor
//-----------------------------------------------------------------------------
CSceneNodeModel::CSceneNodeModel(CCore* Core, string Model, Vector3 Pos,
									Vector3 Rot): ISceneNode(Core)
{
	prv_Core = Core;
	prv_Model = Model;
	prv_Position = Pos;
	prv_Rotation = Rot;
	prv_IsVisible = true;
	
	prv_Core->GetModelLoader()->SetPosition(prv_Position.x, prv_Position.y, prv_Position.z);
	prv_Core->GetModelLoader()->SetRotation(prv_Rotation.x, prv_Rotation.y, prv_Rotation.z);
	prv_BoundingBox = prv_Core->GetModelLoader()->GetBoundingBox(prv_Model);
}

//-----------------------------------------------------------------------------
// Name:		CSceneNodeModel
// Description:	Destructor
//-----------------------------------------------------------------------------
CSceneNodeModel::~CSceneNodeModel()
{
	DeleteNodes();
}

//-----------------------------------------------------------------------------
// Name:		SetVisible
// Description:	Set node visibility
//-----------------------------------------------------------------------------
void CSceneNodeModel::SetVisible(bool Visible)
{
	prv_IsVisible = Visible;
}

//-----------------------------------------------------------------------------
// Name:		SetPosition
// Description:	Set position
//-----------------------------------------------------------------------------					
void CSceneNodeModel::SetPosition(Vector3 Pos)
{
	prv_Position = Pos;
	prv_Core->GetModelLoader()->SetPosition(prv_Position.x, prv_Position.y, prv_Position.z);
	prv_Core->GetModelLoader()->SetRotation(prv_Rotation.x, prv_Rotation.y, prv_Rotation.z);
	prv_BoundingBox = prv_Core->GetModelLoader()->GetBoundingBox(prv_Model);
}

//-----------------------------------------------------------------------------
// Name:		SetRotation
// Description:	Set rotation
//-----------------------------------------------------------------------------
void CSceneNodeModel::SetRotation(Vector3 Rot)
{
	prv_Rotation = Rot;
	prv_Core->GetModelLoader()->SetPosition(prv_Position.x, prv_Position.y, prv_Position.z);
	prv_Core->GetModelLoader()->SetRotation(prv_Rotation.x, prv_Rotation.y, prv_Rotation.z);
	prv_BoundingBox = prv_Core->GetModelLoader()->GetBoundingBox(prv_Model);
}

//-----------------------------------------------------------------------------
// Name:		GetBoundingBox
// Description:	Get thr model BB
//-----------------------------------------------------------------------------
BoundingBox CSceneNodeModel::GetBoundingBox()
{
	return prv_BoundingBox;
}

//-----------------------------------------------------------------------------
// Name:		Update
// Description:	Update all nodes
//-----------------------------------------------------------------------------
void CSceneNodeModel::Update()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
		prv_Nodes[i]->Update();
}

//-----------------------------------------------------------------------------
// Name:		Render
// Description:	Render this node and all child nodes
//-----------------------------------------------------------------------------
void CSceneNodeModel::Render()
{
	if(!prv_IsVisible)
		return;

	RenderNode();
	
	for(int i = 0; i < prv_Nodes.size(); i++)
		prv_Nodes[i]->Render();
}

//-----------------------------------------------------------------------------
// Name:		Render
// Description:	Render this node
//-----------------------------------------------------------------------------
void CSceneNodeModel::RenderNode()
{
	prv_Core->GetModelLoader()->SetPosition(prv_Position.x, prv_Position.y, prv_Position.z);
	prv_Core->GetModelLoader()->SetRotation(prv_Rotation.x, prv_Rotation.y, prv_Rotation.z);
	prv_Core->GetModelLoader()->RenderModel(prv_Model);
}

//-----------------------------------------------------------------------------
// Name:		DeleteNodes
// Description:	Delete all childnodes but NOT THIS NODE!
//-----------------------------------------------------------------------------
void CSceneNodeModel::DeleteNodes()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
	{
		prv_Nodes[i]->DeleteNodes();
		SAFE_DELETE(prv_Nodes[i]);
	}
	prv_Nodes.erase(prv_Nodes.begin(), prv_Nodes.end());
}

//-----------------------------------------------------------------------------
// Name:		GetNodes
// Description:	Get all child nodes
//-----------------------------------------------------------------------------
vector<ISceneNode*>& CSceneNodeModel::GetNodes()
{
	return prv_Nodes;
}

};
