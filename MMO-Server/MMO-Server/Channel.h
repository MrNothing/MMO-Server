#pragma once

#include "Client.h"

using namespace std;

class Channel
{
public:
	Channel(void);
	Channel(int _id, string _name, int _maxClients, bool _autoLeave, bool _isTemp, bool _isPublic);
	~Channel();
	bool join(int clientId);
	bool leave(int clientId);
	bool leave(int clientId, bool notifyAllClients);
	bool kick(int clientId, string reason);
	void kickAll();
	void kickAll(string reason);
	void Send(string type);
	void Send(map<string, string> data);
	void Send(map<string, SerializableObject> data);
	void Send(string key, string value);
	void Send(string type, string key, string value);
	void Send(string type, string key, int value);
	void Send(string type, string key, float value);
	void Send(char* data);
	void SendUsers(int clientId);

	int getId(){return id;}
	string getName(){return name;}
	bool hasAutoLeave(){return leaveIfJoinOtherChannel;}
	bool isTemp(){return r_isTemp;}
	bool isPublic(){return r_isPublic;}
	int getMaxClients(){return maxClients;}
	int getClientsCount(){return users.size();}
private:
	int id;
	string name;
	int maxClients;
	bool leaveIfJoinOtherChannel;
	bool r_isTemp;
	bool r_isPublic;
	map<int, Client*> users;
	map<string, Client*> usersByName;
};
