#pragma once

#include "stdafx.h"

using namespace std;

class Client
{
public:
	Client(void);
	Client(SOCKET _client, sockaddr_in _adress);
	~Client();
	void Send(string type);
	void Send(string key, string value);
	void Send(string type, string key, string value);
	void Send(string type, string key, int value);
	void Send(string type, string key, float value);
	void Send(char* data);
	void LeaveAllChannels();
	void Disconnect();
	void Disconnect(char* message);
	//string getAdress(){string res = inet_ntoa(adress.sin_addr)+""; return res;}
	string getName(){return name;}
	void setName(string _name){name = _name;}
	int getId(){return client;}
	SOCKET getClient(){return client;}
	map<int, int> getChannels(){return _channels;}
private:
	SOCKET client;
	string name;
	sockaddr_in adress;
	map<int, int> _channels;
};
