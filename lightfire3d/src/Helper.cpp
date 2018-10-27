//-----------------------------------------------------------------------------
//  Helper.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Provides Helper Functions -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//------------------------------------------------------------------------------
// Read INI-Like File sections
//------------------------------------------------------------------------------
string ReadConfigString(string File, string Section, string Key)
{
	if (("" == File.c_str()) || ("" == Section.c_str()) || ("" == Key.c_str()))
		return "";

	ifstream FileStream;
	FileStream.open(File.c_str());

	if(!FileStream)
		return "";

	Section = "[" + Section + "]";

	string TempString;
	string TempCurrentSection;
	string::size_type TempPos;
	string TempReturn;
	while(!FileStream.eof())
	{
		getline(FileStream, TempString);
		if((string::npos != (TempPos = TempString.find("]", 0))) &&
			(TempString[0] == '['))
			TempCurrentSection = TempString;

		if((string::npos != (TempPos = TempString.find(Key, 0))) &&
			(TempCurrentSection == Section))
		{
			TempPos = TempString.find("=", 0);
			TempReturn = TempString.substr(TempPos+1, TempString.length() - TempPos);
		}
	}

	FileStream.close();
	return TempReturn;
}

//------------------------------------------------------------------------------
// Find headers of INI-Like Files
//------------------------------------------------------------------------------
vector<string> FindConfigHeader(string File)
{
	vector<string> TempVec;

	if ("" == File.c_str())
		return TempVec;

	ifstream FileStream;
	FileStream.open(File.c_str());
	if(!FileStream)
		return TempVec;

	string TempString;
	string::size_type TempPos;
	while(!FileStream.eof())
	{
		getline(FileStream, TempString);
		if((string::npos != (TempPos = TempString.find("]", 0))) &&
			(TempString[0] == '['))
			TempVec.push_back(TempString.substr(1, TempString.length()-2));
	}

	FileStream.close();
	return TempVec;
}

//------------------------------------------------------------------------------
// Split string into string vector
//------------------------------------------------------------------------------
int SplitString(string Data, string Delimeter, vector<string> *SplitData)
{
    string TempData;
    unsigned int TempPos = 0;
    int TempFoundPos = 0;

    SplitData->clear();

    while(true)
    {
        if(-1 == (TempFoundPos = Data.find(Delimeter, TempPos)))
        {
            TempData = Data.substr(TempPos, Data.length() - TempPos);
            SplitData->push_back(TempData);
	    break;
        }
        TempData = Data.substr(TempPos, TempFoundPos - TempPos);
        TempPos = TempFoundPos + 1;
        SplitData->push_back(TempData);
    }

    return SplitData->size();
}

//------------------------------------------------------------------------------
// -
//  Examle of offsets (1-4 order):
//  -----ההההההה----
//       22222221111
//  444443333333
//------------------------------------------------------------------------------
GLubyte* GetAlphaChannelRect(TGAFILE* TGAFile, int X1, int X2, int Y1, int Y2)
{
	GLubyte* TempRect = new GLubyte[(X2-X1) * (Y2-Y1)];
	
	int count = 0;
	int offset = ((TGAFile->width*TGAFile->height) - (TGAFile->width*(TGAFile->height-Y2)))*4+3;

	for(int a = Y2; a > Y1; a--)
	{
		offset -= (TGAFile->width - X2)*4;
		offset -= (X2-X1)*4;
		
		for(int b = X2; b > X1; b--)
		{
			TempRect[count] = TGAFile->imageData[offset];
			count++;
			offset += 4;
		}
		
		offset -= (X2-X1)*4;
		offset -= X1*4;
	}
	
	return TempRect;
}

//------------------------------------------------------------------------------
// -
//------------------------------------------------------------------------------
int ListDirectoryFiles(string Directory, string Suffix, vector<string> *Files)
{
	Files->clear();
	
	#if defined(LF3DPLATFORM_LINUX)
		struct dirent *stDirectory;
		DIR *pDir = opendir(Directory.c_str());
		if(NULL != pDir)
		{
			while(NULL != (stDirectory = readdir(pDir)))
			{
				string Filename = stDirectory->d_name;
				if((Filename == ".") || (Filename == ".."))
					continue;

				struct stat FileStats;
				string FilenameTest = Directory + "/" + stDirectory->d_name;
				stat(FilenameTest.c_str(), &FileStats);

				int end = Filename.find('.');
				if(Suffix != Filename.substr(end + 1, Filename.length() - end))
					continue;

				if (!S_ISDIR(FileStats.st_mode))
				{
					Files->push_back(Filename);
				}
			}
		}
		closedir(pDir);
	#elif defined(LF3DPLATFORM_WINDOWS)
		WIN32_FIND_DATA FindData;
		HANDLE FindHandle;
		string Pattern = Directory + "/*." + Suffix;
		FindHandle = FindFirstFile(Pattern.c_str(), &FindData);

		if(INVALID_HANDLE_VALUE == FindHandle)
			return 0;

		do
		{
			/*if(strcmp(FindData.cFileName, ".") != 0 &&
				strcmp(FindData.cFileName, "..") != 0)
				FindNextFile(FindHandle, &FindData);

			if(FILE_ATTRIBUTE_DIRECTORY == FindData.dwFileAttributes)
				FindNextFile(FindHandle, &FindData);*/

			Files->push_back(FindData.cFileName);
		} while (FindNextFile(FindHandle, &FindData));
		FindClose(FindHandle);
	#endif
	
	return Files->size();
}

//------------------------------------------------------------------------------
// Snap to next Corner in Grid
//------------------------------------------------------------------------------
float SnapToGrid(float Location, float Grid)
{
	if(0.0 == Grid)
		return Location;
	else
		return floor((Location + 0.5*Grid)/Grid)*Grid;
}

//------------------------------------------------------------------------------
// Draw red BB
//------------------------------------------------------------------------------
void DrawBoundingBox(BoundingBox Box)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);
		glColor3ub(255, 0, 0);			
		glVertex3f(Box.Min.x, Box.Min.y, Box.Max.z); //front bottom
		glVertex3f(Box.Max.x, Box.Min.y, Box.Max.z);		
		glVertex3f(Box.Min.x, Box.Max.y, Box.Max.z); //front top
		glVertex3f(Box.Max.x, Box.Max.y, Box.Max.z);			
		glVertex3f(Box.Min.x, Box.Min.y, Box.Max.z); //front left
		glVertex3f(Box.Min.x, Box.Max.y, Box.Max.z);
		glVertex3f(Box.Max.x, Box.Min.y, Box.Max.z); //front right
		glVertex3f(Box.Max.x, Box.Max.y, Box.Max.z);	
		glVertex3f(Box.Min.x, Box.Min.y, Box.Min.z); //back bottom
		glVertex3f(Box.Max.x, Box.Min.y, Box.Min.z);		
		glVertex3f(Box.Min.x, Box.Max.y, Box.Min.z); //back top
		glVertex3f(Box.Max.x, Box.Max.y, Box.Min.z);			
		glVertex3f(Box.Min.x, Box.Min.y, Box.Min.z); //back left
		glVertex3f(Box.Min.x, Box.Max.y, Box.Min.z);
		glVertex3f(Box.Max.x, Box.Min.y, Box.Min.z); //back right
		glVertex3f(Box.Max.x, Box.Max.y, Box.Min.z);			
		glVertex3f(Box.Min.x, Box.Min.y, Box.Min.z); //left bottom
		glVertex3f(Box.Min.x, Box.Min.y, Box.Max.z);
		glVertex3f(Box.Min.x, Box.Max.y, Box.Min.z); //left top
		glVertex3f(Box.Min.x, Box.Max.y, Box.Max.z);	
		glVertex3f(Box.Max.x, Box.Min.y, Box.Min.z); //right bottom
		glVertex3f(Box.Max.x, Box.Min.y, Box.Max.z);
		glVertex3f(Box.Max.x, Box.Max.y, Box.Min.z); //right top
		glVertex3f(Box.Max.x, Box.Max.y, Box.Max.z);
		glColor3ub(255, 255, 255);
	glEnd();
}

//------------------------------------------------------------------------------
// Check if two boxes intersect
//------------------------------------------------------------------------------
bool BoundingBoxCollision(BoundingBox Box1, BoundingBox Box2)
{
	if(Box1.Max.x < Box2.Min.x || Box1.Min.x > Box2.Max.x)
		return false;
		
	if(Box1.Max.y < Box2.Min.y || Box1.Min.y > Box2.Max.y)
		return false;
		
	if(Box1.Max.z < Box2.Min.z || Box1.Min.z > Box2.Max.z)
		return false;

	return true;
}

};
