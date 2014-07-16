#pragma once
#include "stdafx.h"
#include "utils.h"

using namespace std;

class Entity
{
public:
	Entity(void);
	Entity(int _id, int _channel, string _name, Vector3 _position, float _moveSpeed);
	~Entity(void);

	//send my position to all Clients in my room.
	void sendPosition();
	//send my position to all Clients that have an entity that is in my range.
	void sendPositionToVisibleClients();
	
	//sends all the elements in the data map to all Clients.
	void sendData();
	void sendDataToVisibleClients();

	void moveToPoint(Vector3 point);
	void moveToPoint(float x, float y, float z);
	void moveToPoint(Vector3 point, float speed);
	void moveToPoint(float x, float y, float z, float speed);

	void run();

	void setData(string name, SerializableObject data);
	void setAllData(map<string, SerializableObject> _data);
	SerializableObject getData(string name);
	map<string, SerializableObject> getAllData();

	void setOwner(int _owner){owner=_owner;}

	void updateIndex();
	void clearIndexes();

	void doSomethingWithAllVisibleEntities(function<void(int, SerializableObject)> action, SerializableObject params);

private :

	int id;
	int owner;

	//the channel this entity is in.
	int channel;

	string name;
	
	//Spacial properties
	Vector3 position;
	Vector3 destination;

	//used by the patfinder
	vector<Vector3> paths;
	float moveSpeed;
	
	//all entities that this entity "sees" (meaning that they are in range) are stored here.
	list<int> visibleEntities;
	
	//all entities that this entity sees and that are the incarnation of a client.
	list<int> visibleClientEntities;

	string lastPosId;

	//everytime an element is updated in data, Clients will be notified (either in range or everyone in the room)
	map<string, SerializableObject> data;
};