#pragma once

#include "Client.h"

using namespace std;

class Channel
{
public:
	Channel(void);
	Channel(int _id, string _name, int _maxClients, bool _autoLeave, bool _isTemp);
	~Channel();
	bool join(int clientId);
	bool leave(int clientId);
	bool leave(int clientId, bool notifyAllClients);
	bool kick(int clientId, string reason);
	void kickAll();
	void kickAll(string reason);
	void Send(string type);
	void Send(string key, string value);
	void Send(string type, string key, string value);
	void Send(string type, string key, int value);
	void Send(string type, string key, float value);
	void Send(char* data);

	int getId(){return id;}
	string getName(){return name;}
	bool hasAutoLeave(){return leaveIfJoinOtherChannel;}
	bool isTemp(){return r_isTemp;}
	
private:
	int id;
	string name;
	int maxClients;
	bool leaveIfJoinOtherChannel;
	bool r_isTemp;
	map<int, Client*> users;
	map<string, Client*> usersByName;
};
