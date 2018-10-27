//-----------------------------------------------------------------------------
//  LogFile.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Log File -
//-----------------------------------------------------------------------------

#ifndef __LF3DLOGFILEH__
#define __LF3DLOGFILEH__

namespace LF3D
{

	class CLogFile
	{
		private:
			ofstream	prv_LogFile;
			string		prv_FileName;
			bool		prv_bEnabled;

		public:
						CLogFile(string FileName);
						~CLogFile();
			void		WriteText(const char *Text, ...);
			void		EnableLogFile(bool Enable);
	};

};

#endif
