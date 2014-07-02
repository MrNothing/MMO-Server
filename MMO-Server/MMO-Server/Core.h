#pragma once
#include "stdafx.h"

#include "ws-util.h"
#include "ws-util.cpp"

using namespace std;

int DoWinsock(const char* pcAddress, int nPort);
SOCKET SetUpListener(const char* pcAddress, int nPort);
DWORD WINAPI EchoHandler(void* sd_);
void AcceptConnections(SOCKET ListeningSocket);
bool EchoIncomingPackets(SOCKET sd);

extern void OnClientMessage(SOCKET clientId, char* message);
extern void OnClientDisconnected(SOCKET clientId, int reason);
extern void OnClientConnected(SOCKET clientId, sockaddr_in adress);
extern string serverIp;
extern int serverPort;

int start()
{
	 // Start Winsock up
    WSAData wsaData;
	int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		cerr << "error code " << nCode << "." <<
				endl;
        return 255;
    }
	
	int retval = DoWinsock((const char*)serverIp.c_str(), serverPort);
	
	cout<<"Sever closed...";

	while(true);
}

int DoWinsock(const char* pcAddress, int nPort)
{
    cout << "Establishing the listener..." << endl;
    SOCKET ListeningSocket = SetUpListener(pcAddress, htons(nPort));
    if (ListeningSocket == INVALID_SOCKET) {
        cout << endl << WSAGetLastErrorMessage("establish listener") << 
                endl;
        return 3;
    }
	
	cout <<endl;
	cout <<endl;
	cout <<"***************************************"<<endl;
	cout <<"*                                     *"<<endl;
    cout <<"*   >MrNothing's MMO-Server is UP!<   *" << endl;
	cout <<"*                                     *"<<endl;
	cout <<"***************************************"<<endl;
    while (1) {
        AcceptConnections(ListeningSocket);
        cout << "Acceptor restarting..." << endl;
    }

#if defined(_MSC_VER)
    return 0;   // warning eater
#endif
}

SOCKET SetUpListener(const char* pcAddress, int nPort)
{
    u_long nInterfaceAddr = inet_addr(pcAddress);
    if (nInterfaceAddr != INADDR_NONE) {
        SOCKET sd = _WINSOCK2API_::socket(AF_INET, SOCK_STREAM, 0);
        if (sd != INVALID_SOCKET) {
            sockaddr_in sinInterface;
            sinInterface.sin_family = AF_INET;
            sinInterface.sin_addr.s_addr = nInterfaceAddr;
            sinInterface.sin_port = nPort;
			if (_WINSOCK2API_::bind(sd, (sockaddr*)&sinInterface, 
                    sizeof(sockaddr_in)) != SOCKET_ERROR) {
                listen(sd, SOMAXCONN);
                return sd;
            }
            else {
                cerr << WSAGetLastErrorMessage("bind() failed") <<
                        endl;
            }
        }
    }

    return INVALID_SOCKET;
}

DWORD WINAPI EchoHandler(void* sd_) 
{
    int nRetval = 0;
    SOCKET sd = (SOCKET)sd_;

    if (!EchoIncomingPackets(sd)) {
        cerr << endl << WSAGetLastErrorMessage(
                "Echo incoming packets failed") << endl;
        nRetval = 3;
    }

    cout << "Shutting connection down..." << flush;
    if (ShutdownConnection(sd)) {
        cout << "Connection is down." << endl;
    }
    else {
        cerr << endl << WSAGetLastErrorMessage(
                "Connection shutdown failed") << endl;
        nRetval = 3;
    }

    return nRetval;
}

void AcceptConnections(SOCKET ListeningSocket)
{
    sockaddr_in sinRemote;
    int nAddrSize = sizeof(sinRemote);

    while (1) {
        SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote,
                &nAddrSize);
        if (sd != INVALID_SOCKET) {
            cout << "Accepted connection from " <<inet_ntoa(sinRemote.sin_addr) << ":" <<ntohs(sinRemote.sin_port) << "." <<endl;

			OnClientConnected(sd, sinRemote);

            DWORD nThreadID;
            CreateThread(0, 0, EchoHandler, (void*)sd, 0, &nThreadID);
        }
        else {
            cerr << WSAGetLastErrorMessage("accept() failed") << endl;
            return;
        }
    }
}

bool EchoIncomingPackets(SOCKET sd)
{
    // Read data from client
    char acReadBuffer[kBufferSize];
    int nReadBytes;
    do {
        nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
        if (nReadBytes > 0) {
            
			string parasableMessage = acReadBuffer;
			//cout << "Received " << acReadBuffer << " from client." << sd << endl;
			//cout << "Parsed " << parasableMessage.substr(0, parasableMessage.find("}")).c_str() << endl;
        
			OnClientMessage(sd, (char*)parasableMessage.substr(0, parasableMessage.find("¤")).c_str());
        }
        else if (nReadBytes == SOCKET_ERROR) {
            return false;
        }
    } while (nReadBytes != 0);

    //cout << "Connection closed by peer." << endl;
	OnClientDisconnected(sd, 2);

	return true;
}

bool _Send(char* message, SOCKET sd)
{
	int nReadBytes = strlen(message);
	int nSentBytes = 0;
    while (nSentBytes < nReadBytes) 
	{
        int nTemp = send(sd, message + nSentBytes,
                nReadBytes - nSentBytes, 0);
        if (nTemp > 0) 
		{
            //cout << "Sent " << nTemp << " bytes back to client." << endl;
            nSentBytes += nTemp;
        }
        else if (nTemp == SOCKET_ERROR) 
		{
			OnClientDisconnected(sd, 0);
            return false;
        }
        else 
		{
            // Client closed connection before we could reply to
            // all the data it sent, so bomb out early.
            //cout << "Peer unexpectedly dropped connection!" << endl;
			OnClientDisconnected(sd, 1);
            return true;
        }
    }

	return true;
}

string readFileAsString(char* path)
{
	string result="";
	string line;
	ifstream myfile (path);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			cout<<line.c_str()<<endl;
			result += line;
		}
		myfile.close();
	}

	return result;
}