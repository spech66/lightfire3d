//-----------------------------------------------------------------------------
//  Helper.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Provides Helper Functions -
//-----------------------------------------------------------------------------

#ifndef __LF3DHELPERH__
#define __LF3DHELPERH__

namespace LF3D
{

	string ReadConfigString(string File, string Section, string Key);
	vector<string> FindConfigHeader(string File);
	int SplitString(string Data, string Delimeter, vector<string> *SplitData);
	GLubyte* GetAlphaChannelRect(TGAFILE* TGAFile, int X1, int X2, int Y1, int Y2);
	int ListDirectoryFiles(string Directory, string Suffix, vector<string> *Files);
	float SnapToGrid(float Location, float Grid);
	void DrawBoundingBox(BoundingBox Box);
	bool BoundingBoxCollision(BoundingBox Box1, BoundingBox Box2);
};

#endif
