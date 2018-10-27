//-----------------------------------------------------------------------------
//  NetworkClient.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Network Client -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		CNetworkClient
// Description:	Constructor
//-----------------------------------------------------------------------------
CNetworkClient::CNetworkClient(CCore *Core)
{
	prv_Core = Core;
	prv_Connected = false;
}

//-----------------------------------------------------------------------------
// Name:		Init
// Description:	Init Client
//-----------------------------------------------------------------------------
RESULT CNetworkClient::Init()
{
	prv_Core->GetLogFile()->WriteText("Init: --> NetworkClient");
	
	#if defined(LF3DPLATFORM_LINUX)
	#elif defined(LF3DPLATFORM_WINDOWS)
		WSADATA Data;
		if(0 != WSAStartup(MAKEWORD(2, 0), &Data))
			prv_Core->GetLogFile()->WriteText("Error: WSAStartup() fails - %i", WSAGetLastError());
	#endif
	
	prv_Core->GetLogFile()->WriteText("Init: <-- NetworkClient");

	return OK;
}

//-----------------------------------------------------------------------------
// Name:		Connect
// Description:	Connect to server
//-----------------------------------------------------------------------------
int CNetworkClient::Connect(string Server, int Port)
{
	Close();
	// TCP/IP
	prv_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == prv_Socket)
	{
		prv_Core->GetLogFile()->WriteText("Error: Could not create Socket for Connect");
		return 1;
	}

	memset(&prv_Server, 0, sizeof(prv_Server));
	prv_Server.sin_family = AF_INET;
	prv_Server.sin_port = htons(Port);
	
	if(INADDR_NONE != inet_addr(Server.c_str()))
	{
		prv_Server.sin_addr.s_addr = inet_addr(Server.c_str());
	} else {
		struct hostent* Host = gethostbyname(Server.c_str());
		if(NULL != Host)
		{
			memcpy(&(prv_Server.sin_addr), Host->h_addr_list[0], 4);
			Server = Host->h_addr_list[0];
		} else {
			prv_Core->GetLogFile()->WriteText("Error: Could not resolve Hostname %s", Server.c_str());
			return 1;
		}
	}
	
	if(-1 == connect(prv_Socket, (struct sockaddr*)&prv_Server, sizeof(struct sockaddr)))
	{
		prv_Core->GetLogFile()->WriteText("Error: Connect to Server - %s", strerror(errno));
		return 1;
	}
	
	#if defined(LF3DPLATFORM_WINDOWS)
		#define MSG_DONTWAIT 0
		unsigned long SockMode = 1;
 		ioctlsocket(prv_Socket, FIONBIO, &SockMode);
	#endif
	
	prv_Connected = true;
	prv_Core->GetLogFile()->WriteText("Init: Connected to %s at %i", Server.c_str(), Port);
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name:		ProcessMessages
// Description:	Get Messages
//-----------------------------------------------------------------------------
int CNetworkClient::ProcessMessages(void* Data, int Size)
{
	int Ret;
	#if defined(LF3DPLATFORM_LINUX)
		Ret = recv(prv_Socket, Data, Size, MSG_DONTWAIT);
	#elif defined(LF3DPLATFORM_WINDOWS)
		Ret = recv(prv_Socket, reinterpret_cast<char*>(Data), Size, MSG_DONTWAIT);
	#endif
	
	if(0 == Ret)
	{
		prv_Connected = false;
		prv_Core->GetLogFile()->WriteText("Info: Server connection closed");
	}
	
	return Ret;
}

//-----------------------------------------------------------------------------
// Name:		Send
// Description:	Send Data
//-----------------------------------------------------------------------------
void CNetworkClient::Send(const void* Data, int Size)
{
	if(!prv_Connected)
		return;
	/*char* TempData = const_cast<char*>(reinterpret_cast<const char*>(Data));
	int DataSend = 0;
	int DataLeft = Size;
	int n;
	while(DataSend < DataLeft)
	{
		n = send(prv_Socket, TempData+DataSend, DataLeft, 0);
		if(n == -1)
			break;
		DataSend += n;
		DataLeft -= n;
	}*/
	#if defined(LF3DPLATFORM_LINUX)
		send(prv_Socket, Data, Size, 0);
	#elif defined(LF3DPLATFORM_WINDOWS)
		send(prv_Socket, (char*)(Data), Size, 0);
	#endif
}

//-----------------------------------------------------------------------------
// Name:		Close
// Description:	Close open Sockets
//-----------------------------------------------------------------------------
void CNetworkClient::Close()
{
	#if defined(LF3DPLATFORM_LINUX)
	if(-1 != prv_Socket)
		close(prv_Socket);
	#elif defined(LF3DPLATFORM_WINDOWS)
	if(-1 != prv_Socket)
		closesocket(prv_Socket);
	#endif
}

//-----------------------------------------------------------------------------
// Name:		NetworkClient
// Description:	Destructor
//-----------------------------------------------------------------------------
CNetworkClient::~CNetworkClient()
{
	prv_Core->GetLogFile()->WriteText("Uninit: --> NetworkClient");
	
	#if defined(LF3DPLATFORM_LINUX)
	#elif defined(LF3DPLATFORM_WINDOWS)
	if(SOCKET_ERROR == WSACleanup())
		prv_Core->GetLogFile()->WriteText("Error: WSACleanup() fails");
	#endif
	
	prv_Core->GetLogFile()->WriteText("Uninit: <-- NetworkClient");
}

};
