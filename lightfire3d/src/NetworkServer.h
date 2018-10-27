//-----------------------------------------------------------------------------
//  NetworkServer.h
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Network Server -
//-----------------------------------------------------------------------------

#ifndef __LF3DNETWORKSERVERH__
#define __LF3DNETWORKSERVERH__

namespace LF3D
{

	class CNetworkServer
	{
		private:
			//! Pointer to CoreDevice
			CCore			*prv_Core;
			int				prv_AcceptSocket;
			int 			*prv_Clients;
			int				prv_MaxClients;
			fd_set 			prv_FdSet;

		public:
			//! Constructor
					CNetworkServer(CCore *Core);
			//! Destructor
					~CNetworkServer();
			RESULT	Init();
			int 	Listen(int Port);
			int		Update();
			RESULT	ProcessMessages(void* Data, int Size, int &ClientID);
			void 	Send(const void* Data, int Size);
			void 	Send(const void* Data, int Size, int ClientID);
			void 	Close();
			void	SetMaximumClients(int Max);
	};

};

#endif
