//-----------------------------------------------------------------------------
//  NetworkServer.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Network Server -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include <sstream>

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CNetworkServer
// Description:	Constructor
//-----------------------------------------------------------------------------
CNetworkServer::CNetworkServer(CCore *Core)
{
	prv_Core 			= Core;
	prv_AcceptSocket 	= -1;
	prv_Clients			= NULL;
	SetMaximumClients(64);
}

//-----------------------------------------------------------------------------
// Name:		Init
// Description:	Init Server
//-----------------------------------------------------------------------------
RESULT CNetworkServer::Init()
{
	prv_Core->GetLogFile()->WriteText("Init: --> NetworkServer");
	
	#if defined(LF3DPLATFORM_LINUX)
	#elif defined(LF3DPLATFORM_WINDOWS)
		WSADATA Data;
		if(0 != WSAStartup(MAKEWORD(2, 0), &Data))
			prv_Core->GetLogFile()->WriteText("Error: WSAStartup() fails - %i", WSAGetLastError());
	#endif
	
	prv_Core->GetLogFile()->WriteText("Init: <-- NetworkServer");

	return OK;
}

//-----------------------------------------------------------------------------
// Name:		Init
// Description:	Init Server
//-----------------------------------------------------------------------------
int CNetworkServer::Listen(int Port)
{
	struct sockaddr_in Address;
	
	Close();
	prv_AcceptSocket = socket(AF_INET, SOCK_STREAM, 0);	// TCP/IP
	if(-1 == prv_AcceptSocket)
	{
		prv_Core->GetLogFile()->WriteText("Error: Could not create Socket for Listen");
		return 1;
	}
	
	memset(&Address, 0, sizeof(Address));
	Address.sin_family = AF_INET;
	Address.sin_port = htons(Port);
	Address.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1 == bind(prv_AcceptSocket, (struct sockaddr*)&Address, sizeof(Address)))
	{
		prv_Core->GetLogFile()->WriteText("Error: Could not bind Socket for Listen");
		return 1;
	}
	
	if(-1 == listen(prv_AcceptSocket, 2))
	{
		prv_Core->GetLogFile()->WriteText("Error: Listen on Port");
	}
	
	prv_Core->GetLogFile()->WriteText("Info: Listening on Port %i", Port);

	return 0;
}

//-----------------------------------------------------------------------------
// Name:		Update
// Description:	Handle connections, incoming messages, ...
//-----------------------------------------------------------------------------
int CNetworkServer::Update()
{
	FD_ZERO(&prv_FdSet);
	
	FD_SET(prv_AcceptSocket, &prv_FdSet);
	for(int i = 0; i < prv_MaxClients; i++)
	{
		if(-1 != prv_Clients[i])
			FD_SET(prv_Clients[i], &prv_FdSet);
	}
	
	int Errorcode;
	#if defined(LF3DPLATFORM_LINUX)
	struct timeval TimeVal;
	TimeVal.tv_sec = 0;
	TimeVal.tv_usec = 1;
	Errorcode = select(prv_MaxClients+1, &prv_FdSet, NULL, NULL, &TimeVal);
	#elif defined(LF3DPLATFORM_WINDOWS)
	Errorcode = select(0, &prv_FdSet, NULL, NULL, NULL);
	#endif
	if(-1 == Errorcode)
	{
		prv_Core->GetLogFile()->WriteText("Error: Network select");
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name:		ProcessMessages
// Description:	Get Messages
//-----------------------------------------------------------------------------
RESULT CNetworkServer::ProcessMessages(void* Data, int Size, int &ClientID)
{
	//Handle new Clients
	if(FD_ISSET(prv_AcceptSocket, &prv_FdSet))
	{
		FD_CLR(prv_AcceptSocket, &prv_FdSet);
		for(int i = 0; i < prv_MaxClients; i++)
		{
			if(-1 == prv_Clients[i])
			{
				prv_Clients[i] = accept(prv_AcceptSocket, NULL, NULL);
				prv_Core->GetLogFile()->WriteText("Info: New Client accepted");
				ClientID = i;
				return NET_NEWCONNECTION;
			}
		}
	}

	//Process Messages and Connections
	for(int i = 0; i < prv_MaxClients; i++)
	{
		if(-1 == prv_Clients[i])
			continue;

		if(FD_ISSET(prv_Clients[i], &prv_FdSet))
		{
			FD_CLR(prv_Clients[i], &prv_FdSet);
			int Len;
			#if defined(LF3DPLATFORM_LINUX)
				Len = recv(prv_Clients[i], Data, Size, 0);
			#elif defined(LF3DPLATFORM_WINDOWS)
				Len = recv(prv_Clients[i], reinterpret_cast<char*>(Data), Size, 0);
			#endif
			if(0 == Len || -1 == Len)
			{
				prv_Core->GetLogFile()->WriteText("Info: Client connection closed");
				#if defined(LF3DPLATFORM_LINUX)
				close(prv_Clients[i]);
				#elif defined(LF3DPLATFORM_WINDOWS)
				closesocket(prv_Clients[i]);
				#endif
				prv_Clients[i] = -1;
				ClientID = i;
				return NET_CLOSE;
			}
			ClientID = i;
			return NET_MESSAGE;
		}
	}
	
	return OK;
}

//-----------------------------------------------------------------------------
// Name:		Send
// Description:	Send data to all Clients
//-----------------------------------------------------------------------------
void CNetworkServer::Send(const void* Data, int Size)
{
	char* TempData = const_cast<char*>(reinterpret_cast<const char*>(Data));
	for(int i = 0; i < prv_MaxClients; i++)
	{
		if(-1 != prv_Clients[i])
		{	
			int DataSend = 0;
			int DataLeft = Size;
			int n;
			while(DataSend < DataLeft)
			{
				n = send(prv_Clients[i], TempData+DataSend, DataLeft, 0);
				if(n == -1)
					break;
				DataSend += n;
				DataLeft -= n;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name:		Send
// Description:	Send data to a Client
//-----------------------------------------------------------------------------
void CNetworkServer::Send(const void* Data, int Size, int ClientID)
{
	if(-1 == prv_Clients[ClientID])
		return;

	char* TempData = const_cast<char*>(reinterpret_cast<const char*>(Data));
	int DataSend = 0;
	int DataLeft = Size;
	int n;
	while(DataSend < DataLeft)
	{
		n = send(prv_Clients[ClientID], TempData+DataSend, DataLeft, 0);
		if(n == -1)
			break;
		DataSend += n;
		DataLeft -= n;
	}
}

//-----------------------------------------------------------------------------
// Name:		Close
// Description:	Close open sockets
//-----------------------------------------------------------------------------
void CNetworkServer::Close()
{
	#if defined(LF3DPLATFORM_LINUX)
	if(-1 != prv_AcceptSocket)
		close(prv_AcceptSocket);
		
	for(int i = 0; i < prv_MaxClients; i++)
	{
		if(-1 != prv_Clients[i])
			close(prv_Clients[i]);
	}
	#elif defined(LF3DPLATFORM_WINDOWS)
	if(-1 != prv_AcceptSocket)
		closesocket(prv_AcceptSocket);
		
	for(int i = 0; i < prv_MaxClients; i++)
	{
		if(-1 != prv_Clients[i])
			closesocket(prv_Clients[i]);
	}
	#endif
}

//-----------------------------------------------------------------------------
// Name:		SetMaximumClients
// Description:	Set maximum clients
//-----------------------------------------------------------------------------
void CNetworkServer::SetMaximumClients(int Max)
{
	if(NULL == prv_Clients)
	{
		prv_MaxClients = Max;
		prv_Clients = new int[prv_MaxClients];
		for(int i = 0; i < prv_MaxClients; i++)
			prv_Clients[i] = -1;
	} else {
		int* TempClients = prv_Clients;
		prv_Clients = new int[Max];
		for(int i = 0; i < Max; i++)
		{
			if(i <= prv_MaxClients)
				prv_Clients[i] = TempClients[i];
			else
				prv_Clients[i] = -1;
		}
			
		prv_MaxClients = Max;
		SAFE_DELETE(TempClients);
	}
}

//-----------------------------------------------------------------------------
// Name:		CNetworkServer
// Description:	Destructor
//-----------------------------------------------------------------------------
CNetworkServer::~CNetworkServer()
{
	prv_Core->GetLogFile()->WriteText("Uninit: --> NetworkServer");
	
	#if defined(LF3DPLATFORM_LINUX)
	#elif defined(LF3DPLATFORM_WINDOWS)
	if(SOCKET_ERROR == WSACleanup())
		prv_Core->GetLogFile()->WriteText("Error: WSACleanup() fails - %i", WSAGetLastError());
	#endif
	
	prv_Core->GetLogFile()->WriteText("Uninit: <-- NetworkServer");
}

};
