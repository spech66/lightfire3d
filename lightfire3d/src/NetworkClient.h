//-----------------------------------------------------------------------------
//  NetworkClient.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Network Client -
//-----------------------------------------------------------------------------

#ifndef __LF3DNETWORKCLIENTH__
#define __LF3DNETWORKCLIENTH__

namespace LF3D
{

	class CNetworkClient
	{
		private:
			//! Pointer to CoreDevice
			CCore				*prv_Core;
			int					prv_Socket;
			struct sockaddr_in 	prv_Server;
			bool				prv_Connected;

		public:
			//! Constructor
					CNetworkClient(CCore *Core);
			//! Destructor
					~CNetworkClient();
			RESULT	Init();
			int 	Connect(string Server, int Port);
 			int 	ProcessMessages(void* Data, int Size);
			void 	Send(const void* Data, int Size);
			void 	Close();
	};

};

#endif
