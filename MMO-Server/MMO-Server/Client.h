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
	void BroadCast(map<string, SerializableObject> data);
	void Send(string type);
	void Send(map<string, string> data);
	void Send(map<string, SerializableObject> data);
	template<typename T>void Send(Object<T> data);
	void Send(string key, string value);
	void Send(string type, string key, string value);
	void Send(string type, string key, int value);
	void Send(string type, string key, float value);
	void Send(char* data);
	void SendPersistentData(string name);
	void SendAllPersistentData();
	void SendAllPublicData();
	void SetPublicData(string name, SerializableObject data);
	void SetPersistentData(string name, SerializableObject data);
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
	bool hasDisconnected(){return c_hasDisconnected;}
	void setDisconnected(bool value){c_hasDisconnected = value;}

	string serializableObjectToString(map<string, SerializableObject> data);
	template<typename T>string ObjectToString(Object<T> data);
	
private:
	SOCKET client;
	string name;
	sockaddr_in adress;
	map<int, int> _channels;
	bool c_hasDisconnected;

	//Public data is data that is sent to all players in the channels when updated (or on the first join).
	map<string, SerializableObject> publicData;

	//Persistent data is data that will remain even if the client disconnects or the server is closed. The data is stored in the "Persistent" folder and is associated to the client's username.
	map<string, SerializableObject> persistentData;
};
