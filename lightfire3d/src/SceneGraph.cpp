//-----------------------------------------------------------------------------
//  SceneGraph.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Manage scene nodes -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CSceneGraph
// Description:	Contructor
//-----------------------------------------------------------------------------
CSceneGraph::CSceneGraph(CCore *Core)
{
	prv_Core = Core;
}

//-----------------------------------------------------------------------------
// Name:		~CSceneGraph
// Description:	Destructor
//-----------------------------------------------------------------------------
CSceneGraph::~CSceneGraph()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
	{
		prv_Nodes[i]->DeleteNodes();
		SAFE_DELETE(prv_Nodes[i]);
	}
	prv_Nodes.erase(prv_Nodes.begin(), prv_Nodes.end());
}

//-----------------------------------------------------------------------------
// Name:		AddModelNode
// Description:	Add an Model
//-----------------------------------------------------------------------------
ISceneNode* CSceneGraph::AddModelNode(string Model, Vector3 Pos, Vector3 Rot)
{
	ISceneNode *TempNode = new CSceneNodeModel(prv_Core, Model, Pos, Rot);
	prv_Nodes.push_back(TempNode);
	return TempNode;
}

//-----------------------------------------------------------------------------
// Name:		AddOctreeNode
// Description:	Add an Octree
//-----------------------------------------------------------------------------
ISceneNode* CSceneGraph::AddOctreeNode()
{
	ISceneNode *TempNode = new CSceneNodeOctree(prv_Core);
	prv_Nodes.push_back(TempNode);
	return TempNode;
}

//-----------------------------------------------------------------------------
// Name:		Render
// Description:	Call all render functions
//-----------------------------------------------------------------------------
void CSceneGraph::Render()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
		prv_Nodes[i]->Render();
}

//-----------------------------------------------------------------------------
// Name:		Update
// Description:	Call all update functions
//-----------------------------------------------------------------------------
void CSceneGraph::Update()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
		prv_Nodes[i]->Update();
}

};
