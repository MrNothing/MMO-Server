#include "stdafx.h"
#include "Client.h"
#include "Channel.h"

extern bool _Send(char* message, SOCKET sd);
extern map<int, Channel*> channels;

typedef std::map<int, int>::iterator ChannelIterator;

Client::Client(SOCKET _client, sockaddr_in _adress)
{
	client = _client;
	adress = _adress; 
	name = "";
}

void Client::Send(string data)
{
	_Send((char*)data.c_str(), client);
}

void Client::Send(string key, string value)
{
	string data = "{\""+key+"\": \""+value+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(string type, string key, string value)
{
	string data = "{\"type\": \""+type+"\", \""+key+"\": \""+value+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(string type, string key, int value)
{
	//_Send("{\"type\": \""+type.c_str()+"\", \""+key+"\": \""+value+"\"}", client);
}

void Client::Send(string type, string key, float value)
{
	//_Send("{\"type\": \""+type.c_str()+"\", \""+key+"\": \""+value+"\"}", client);
}

void Client::Send(char* data)
{
	_Send(data, client);
}

void Client::LeaveAllChannels()
{
	for(ChannelIterator iterator = this->_channels.begin(); iterator != this->_channels.end(); iterator++) 
	{
		channels[iterator->second]->leave(client);
	}
}