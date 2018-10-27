//-----------------------------------------------------------------------------
//  LogFile.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Log File -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CLogFile
// Description:	Constructor
//-----------------------------------------------------------------------------
CLogFile::CLogFile(string FileName)
{
	if("" == FileName)
		FileName = "logfile.txt";

	prv_FileName = FileName;
	prv_bEnabled = true;

	//Get current time and date
	char tmpbuf[128];
	ZeroMemory(tmpbuf, sizeof(tmpbuf));
	time_t ltime;
	time(&ltime);
	struct tm *today;
	today = localtime(&ltime);
    strftime(tmpbuf, 128, "%H:%M:%S - %d.%m.%Y", today);

	prv_LogFile.open(prv_FileName.c_str(), ios::out);
	prv_LogFile << "Log: Log file opened, " << tmpbuf << endl;
	#if defined(LF3DPLATFORM_LINUX)
		prv_LogFile << "Code compiled for Linux" << endl;
	#elif defined(LF3DPLATFORM_WINDOWS)
		prv_LogFile << "Code compiled for Windows" << endl;
	#endif
	prv_LogFile.close();
}

//-----------------------------------------------------------------------------
// Name:		WriteText
// Description:	Write the Text to the Log File - works like printf
//-----------------------------------------------------------------------------
void CLogFile::WriteText(const char *Text, ...)
{
	if(prv_bEnabled)
	{
		static char StringBuffer[1024];
		va_list args;
		va_start( args, Text );
		_vsnprintf(StringBuffer, 1024*sizeof(char), Text, args);

		prv_LogFile.open(prv_FileName.c_str(), ios::app);
		prv_LogFile << StringBuffer << endl;
		prv_LogFile.close();

		va_end(args);
	}
}

//-----------------------------------------------------------------------------
// Name:		EnableLogFile
// Description:	Enable\Disable LogFile
//-----------------------------------------------------------------------------
void CLogFile::EnableLogFile(bool Enable)
{
	prv_bEnabled = Enable;
}

//-----------------------------------------------------------------------------
// Name:		CLogFile
// Description:	Destructor
//-----------------------------------------------------------------------------
CLogFile::~CLogFile()
{
	//Get current time and date
	char tmpbuf[128];
	ZeroMemory(tmpbuf, sizeof(tmpbuf));
	time_t ltime;
	time(&ltime);
	struct tm *today;
	today = localtime(&ltime);
    strftime(tmpbuf, 128, "%H:%M:%S - %d.%m.%Y", today);

	prv_LogFile.open(prv_FileName.c_str(), ios::app);
	prv_LogFile << "Log: Log file closed, " << tmpbuf << endl;
	prv_LogFile.close();
}

};

