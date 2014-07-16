#pragma once

#include "stdafx.h"
#include "Channel.h"

extern void destroyChannel(int id);
extern map<int, Client*> clients;
extern map<int, Channel*> channels;

typedef std::map<int, int>::iterator ChannelIterator;
typedef std::map<int, Client*>::iterator ClientIterator;
typedef std::map<int, Entity*>::iterator EntityIterator;

Channel::Channel(int _id, string _name, int _maxClients, bool _autoLeave, bool _isTemp, bool _isPublic)
{
	id = _id;
	name = _name;
	maxClients = _maxClients;
	leaveIfJoinOtherChannel = _autoLeave;
	r_isTemp = _isTemp;
	r_isPublic = _isPublic;

	viewRange = Vector3i(1, 1, 1);
	indexBlocSize = 20;

	optimizedSynchronization = true;
}

Channel::Channel(void)
{

}

Channel::~Channel()
{

}

void Channel::run()
{
	for(EntityIterator iterator = entities.begin(); iterator != entities.end(); iterator++) 
	{
		iterator->second->run();
	}
}

bool Channel::join(int clientId)
{
	if(clients[clientId]->getName().length()==0)
	{
		clients[clientId]->Send("err", "id", "10"); //You must be logged in first!
		return false;
	}
	
	if(!exists<int, Client*>(users, clientId))
	{
		//leave every channel if not persistent...
		map<int, int> clientChannels = clients[clientId]->getChannels();
		for(ChannelIterator iterator = clientChannels.begin(); iterator != clientChannels.end(); iterator++) 
		{
			if(channels[iterator->second]->hasAutoLeave())
				channels[iterator->second]->leave(clientId);
		}

		clientChannels[getId()] = getId();

		clients[clientId]->setChannels(clientChannels);

		users[clientId] = clients[clientId];
		usersByName[clients[clientId]->getName()] = clients[clientId];

		map<string, string> clientInfos;
		clientInfos["type"] = "join";
		clientInfos["id"] =	to_string(clients[clientId]->getId());
		clientInfos["name"] = clients[clientId]->getName();
		clientInfos["chan_id"] = to_string(id);
		clientInfos["chan"] = name;

		Send(clientInfos);

		return true;
	}
	else
	{
		clients[clientId]->Send("err", "id", "9"); //You have already joined this channel
		return true;
	}
}

bool Channel::leave(int clientId)
{
	if(exists<int, Client*>(users, clientId))
	{
		if(!clients[clientId]->hasDisconnected())
		{
			map<string, string> data;
			data["type"] = "leave";
			data["id"] = to_string(clients[clientId]->getId());
			data["name"] = clients[clientId]->getName();
			data["chan_id"] = to_string(id);
			data["chan"] = name;

			Send(data);
		}

		map<int, int> clientChannels = clients[clientId]->getChannels();
		
		if(exists<int, int>(clientChannels, getId()))
			clientChannels.erase(getId());
		
		clients[clientId]->setChannels(clientChannels);

		users.erase(clientId);
		usersByName.erase(clients[clientId]->getName());

		if(users.size()==0 && r_isTemp)
			destroyChannel(id);

		return true;
	}
	else
	{
		clients[clientId]->Send("err", "id", "8"); //You are not in this room!
		return false;
	}
}

bool Channel::leave(int clientId, bool notifyAllClients)
{
	if(exists<int, Client*>(users, clientId))
	{
		if(notifyAllClients)
		{
			map<string, string> data;
			data["type"] = "leave";
			data["id"] = to_string(clients[clientId]->getId());
			data["name"] = clients[clientId]->getName();
			data["chan_id"] = to_string(id);
			data["chan"] = name;

			Send(data);
		}

		users.erase(clientId);
		usersByName.erase(clients[clientId]->getName());

		if(users.size()==0 && r_isTemp)
			destroyChannel(id);

		return true;
	}
	else
	{
		clients[clientId]->Send("err", "id", "8"); //You are not in this room!
		return false;
	}
}

void Channel::kickAll()
{
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		leave(iterator->second->getId(), false);

		map<string, string> data;
		data["type"] = "leave";
		data["id"] = to_string(iterator->second->getId());
		data["name"] = iterator->second->getName();
		data["chan_id"] = to_string(id);
		data["chan"] = name;

		Send(data);

		iterator->second->Send("type", "leavek");
	}
}

void Channel::Send(string key, string value)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(key, value);
	}
}

void Channel::Send(string type, string key, string value)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(type, key, value);
	}
}

void Channel::Send(string type, string key, int value)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(type, key, value);
	}
}

void Channel::Send(string type, string key, float value)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(type, key, value);
	}
}

void Channel::Send(string data)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(data);
	}
}

void Channel::Send(map<string, string> data)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(data);
	}
}

void Channel::Send(map<string, SerializableObject> data)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(data);
	}
}

void Channel::Send(SerializableObject data)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(data);
	}
}

void Channel::SendUsers(int clientId)
{
	map<string, SerializableObject> data;

	map<string, SerializableObject> usersInfos;
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		map<string, SerializableObject> userInfos;
		userInfos["id"] = SerializableObject(iterator->second->getId());
		userInfos["name"] = SerializableObject(iterator->second->getName());

		//TODO: add more infos...
		usersInfos[to_string(iterator->second->getId())] = userInfos;
	}

	data["type"] = SerializableObject("usersList");
	data["users"] = SerializableObject(usersInfos);

	clients[clientId]->Send(data);
}