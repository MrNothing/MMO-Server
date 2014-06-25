#include "stdafx.h"
#include "Channel.h"
//#include "Core.h"

extern void destroyChannel(int id);
extern map<int, Client*> clients;
extern map<int, Channel*> channels;

template<typename T1, typename T2>bool exists(map<T1, T2> m, T1 key)
{
	if ( m.find(key) == m.end() ) {
	  return false;
	} else {
	  return true;
	}
}

typedef std::map<int, int>::iterator ChannelIterator;
typedef std::map<int, Client*>::iterator ClientIterator;

Channel::Channel(int _id, string _name, int _maxClients, bool _autoLeave, bool _isTemp)
{
	id = _id;
	name = _name;
	maxClients = _maxClients;
	leaveIfJoinOtherChannel = _autoLeave;
	r_isTemp = _isTemp;
}

Channel::Channel(void)
{

}

bool Channel::join(int clientId)
{
	if(clients[clientId]->getName().length()==0)
	{
		clients[clientId]->Send("err", "msg", "You must be logged in first!");
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

		Send("join", "user", clients[clientId]->getName());

		users[clientId] = clients[clientId];
		usersByName[clients[clientId]->getName()] = clients[clientId];
		return true;
	}
	else
	{
		clients[clientId]->Send("err", "msg", "You have already joined this channel!");
		return true;
	}
}

bool Channel::leave(int clientId)
{
	if(exists<int, Client*>(users, clientId))
	{
		Send("leave", "user", clients[clientId]->getName());

		users.erase(clientId);
		usersByName.erase(clients[clientId]->getName());

		if(users.size()==0 && r_isTemp)
			destroyChannel(id);

		return true;
	}
	else
	{
		clients[clientId]->Send("err", "msg", "You are not in this room!");
		return false;
	}
}

bool Channel::leave(int clientId, bool notifyAllClients)
{
	if(exists<int, Client*>(users, clientId))
	{
		if(notifyAllClients)
			Send("leave", "user", clients[clientId]->getName());

		users.erase(clientId);
		usersByName.erase(clients[clientId]->getName());

		if(users.size()==0 && r_isTemp)
			destroyChannel(id);

		return true;
	}
	else
	{
		clients[clientId]->Send("err", "msg", "You are not in this room!");
		return false;
	}
}

void Channel::kickAll()
{
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		leave(iterator->second->getId(), false);
		iterator->second->Send("type", "leave");
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

void Channel::Send(string data)
{
	//leave every channel if not persistent...
	for(ClientIterator iterator = users.begin(); iterator != users.end(); iterator++) 
	{
		clients[iterator->first]->Send(data);
	}
}