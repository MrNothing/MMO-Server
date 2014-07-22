#pragma once

#include "Client.h"

using namespace std;

class Channel
{
public:
	Channel(void);
	Channel(int _id, string _name, int _maxClients, bool _autoLeave, bool _isTemp, bool _isPublic);
	~Channel();

	void run();
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
	void Send(SerializableObject data);
	void SendUsers(int clientId);

	int getId(){return id;}
	string getName(){return name;}
	bool hasAutoLeave(){return leaveIfJoinOtherChannel;}
	bool isTemp(){return r_isTemp;}
	bool isPublic(){return r_isPublic;}
	int getMaxClients(){return maxClients;}
	int getClientsCount(){return users.size();}
	float getIndexBlocSize(){return indexBlocSize;}
	bool isOptimizingSync(){return optimizedSynchronization;}

	void setOptimisation(bool status){optimizedSynchronization = status;}

	map<int, Entity*> getEntities(){return entities;}
	map<string, map<int, int>*> getSpacialEntityIndex(){return spacialEntityIndex;}
	map<string, map<int, int>*> getSpacialClientIndex(){return spacialClientIndex;}
	Vector3i getViewRange(){return viewRange;}

	Entity* addEntity(string name, Vector3 position, float moveSpeed);

private:
	int id;
	string name;
	int maxClients;
	bool leaveIfJoinOtherChannel;
	bool r_isTemp;
	bool r_isPublic;
	map<int, Client*> users;
	map<string, Client*> usersByName;

	//Custom Room Data
	map<string, SerializableObject> data;

	/*
	*	--- Game properties ---
	*
	*	These properties are usually used in games.
	*/

	//the entities in this Channel
	map<int, Entity*> entities;

	//Spacial indexation:

	//this is where we store our entities for optimisation.
	map<string, map<int, int>*> spacialEntityIndex;
	//this map only has entities that incarnate Clients in its list.
	map<string, map<int, int>*> spacialClientIndex;
	float indexBlocSize;
	Vector3i viewRange;

	//when this is true, the entities in this channel will send their informations only to players that have an entity in their range.
	bool optimizedSynchronization;
	
	int entityCounter;
};
