#pragma once

#include "stdafx.h"
#include "utils.h"

using namespace std;

class Client
{
public:
	Client(void);
	Client(SOCKET _client, sockaddr_in _adress);
	~Client();
	void Send(string type);
	void Send(map<string, string> data);
	void Send(map<string, SerializableObject> data);
	void Send(string key, string value);
	void Send(string type, string key, string value);
	void Send(string type, string key, int value);
	void Send(string type, string key, float value);
	void Send(char* data);
	string serializableObjectToString(map<string, SerializableObject> data);
	void LeaveAllChannels();
	void Disconnect();
	void Disconnect(char* message);
	//string getAdress(){string res = inet_ntoa(adress.sin_addr)+""; return res;}
	string getName(){return name;}
	void setName(string _name){name = _name;}
	int getId(){return client;}
	SOCKET getClient(){return client;}
	map<int, int> getChannels(){return _channels;}
	void setChannels(map<int, int> chans){_channels = chans;}
private:
	SOCKET client;
	string name;
	sockaddr_in adress;
	map<int, int> _channels;
};
