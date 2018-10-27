//-----------------------------------------------------------------------------
//  SceneNode.cpp
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
// Name:		ISceneNode
// Description:	Contructor
//-----------------------------------------------------------------------------
ISceneNode::ISceneNode(CCore* Core)
{
	prv_Core = Core;
}

//-----------------------------------------------------------------------------
// Name:		AddModelNode
// Description:	Add an Model
//-----------------------------------------------------------------------------
ISceneNode* ISceneNode::AddModelNode(string Model, Vector3 Pos, Vector3 Rot)
{
	ISceneNode *TempNode = new CSceneNodeModel(prv_Core, Model, Pos, Rot);
	prv_Nodes.push_back(TempNode);
	return TempNode;
}

//-----------------------------------------------------------------------------
// Name:		AddOctreeNode
// Description:	Add an Octree
//-----------------------------------------------------------------------------
ISceneNode* ISceneNode::AddOctreeNode()
{
	ISceneNode *TempNode = new CSceneNodeOctree(prv_Core);
	prv_Nodes.push_back(TempNode);
	return TempNode;
}

//-----------------------------------------------------------------------------
// Name:		*
// Description:	virtual functions
//-----------------------------------------------------------------------------
void ISceneNode::SetVisible(bool Visible) {}
void ISceneNode::SetPosition(int x, int y, int z) {}
void ISceneNode::SetRotation(int x, int y, int z) {}

//-----------------------------------------------------------------------------
// Name:		GetBoundingBox
// Description:	Return empty BB
//-----------------------------------------------------------------------------
BoundingBox	ISceneNode::GetBoundingBox()
{
	BoundingBox Temp;
	return Temp;
}

};
