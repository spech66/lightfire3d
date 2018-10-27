//-----------------------------------------------------------------------------
//  Main.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Main File -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"

struct strSampleMessage
{
	unsigned int 	ID;
	unsigned short 	Lenght;
	char 			Data[1024];
};

//-----------------------------------------------------------------------------
// main - Program entry point
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//Create a core instance and get all Devices
	LF3D::CCore 			Core;
	LF3D::CNetworkClient	*NetworkClient 	= Core.GetNetworkClient();
	LF3D::CNetworkServer	*NetworkServer 	= Core.GetNetworkServer();
	
	bool Server;
	if(argc == 2)
	{
		Server = true;
		NetworkServer->Listen(atoi(argv[1]));
	} else if(argc == 3) {
		Server = false;
		NetworkClient->Connect(argv[1], atoi(argv[2]));
	} else {
		cout << "Server: " << argv[0] << " Port" << endl;
		cout << "Client: " << argv[0] << " Server Port" << endl;
		return 0;
	}
	
	bool Running = true;
	int ID = 0;
	while(Running)
	{
		if(true == Server)
		{	
			int Size = sizeof(unsigned int) + sizeof(unsigned short) + 1024;
			NetworkServer->Update();
			LF3D::RESULT Res = LF3D::ERROR;
			do
			{
				strSampleMessage Test;
				ZeroMemory(&Test.Data, sizeof(Test.Data));
				Res = NetworkServer->ProcessMessages(&Test, Size);
				switch(Res)
				{
					case LF3D::NET_NEWCONNECTION:
					{
						cout << "Client joined!" << endl;
					} break;
					case LF3D::NET_CLOSE:
					{
						cout << "Client left!" << endl;
					} break;
					case LF3D::NET_MESSAGE:
					{
						cout << "Incoming: #" << Test.ID << " " << Test.Lenght << " " << Test.Data << endl;
						if(0 == strncmp(Test.Data, "exit", 4))
							Running = false;
					} break;
					default:
						break;
				}
			} while (LF3D::OK != Res);
		} else {
			strSampleMessage Send;
			cin.getline(Send.Data, 1024);
			Send.ID = ID;
			Send.Lenght = strlen(Send.Data);
			int Size = sizeof(Send.ID) + sizeof(Send.Lenght) + Send.Lenght;
			NetworkClient->Send(&Send, Size);
			ID++;
		}
	}
	
	if(true == Server)
		NetworkServer->Close();
	else
		NetworkClient->Close();

	return 0;
}
