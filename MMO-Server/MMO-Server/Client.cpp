#pragma once

#include "stdafx.h"
#include "Client.h"
#include "Channel.h"

string serializableObjectToString(map<string, SerializableObject> data);
extern bool _Send(char* message, SOCKET sd);
extern map<int, Channel*> channels;

typedef std::map<int, int>::iterator ChannelIterator;
typedef std::map<string, string>::iterator stringMapIterator;

Client::Client(SOCKET _client, sockaddr_in _adress)
{
	client = _client;
	adress = _adress; 
	name = "";
	c_hasDisconnected = false;
}

Client::Client(void)
{

}

Client::~Client()
{

}

void Client::Send(string data)
{
	if(hasDisconnected())
		return;

	_Send((char*)data.c_str(), client);
}

void Client::Send(string key, string value)
{
	if(hasDisconnected())
		return;

	string data = "{\""+key+"\": \""+value+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(string type, string key, string value)
{
	if(hasDisconnected())
		return;

	string data = "{\"type\": \""+type+"\", \""+key+"\": \""+value+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(string type, string key, int value)
{
	if(hasDisconnected())
		return;

	string data = "{\"type\": \""+type+"\", \""+key+"\": \""+to_string(value)+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(string type, string key, float value)
{
	if(hasDisconnected())
		return;

	string data = "{\"type\": \""+type+"\", \""+key+"\": \""+to_string(value)+"\"}";
	_Send((char*)data.c_str(), client);
}

void Client::Send(char* data)
{
	if(hasDisconnected())
		return;

	_Send(data, client);
}

void Client::Send(map<string, string> data)
{
	if(hasDisconnected())
		return;

	string message="{";

	int counter = 0;
	for(stringMapIterator iterator = data.begin(); iterator != data.end(); iterator++) 
	{
		message+="\""+iterator->first+"\": \""+iterator->second+"\"";
		
		if(counter<data.size()-1)
			message+=",";

		counter++;
	}
	message+="}";

	_Send((char*)message.c_str(), client);
}

void Client::Send(map<string, SerializableObject> data)
{
	string message=serializableObjectToString(data);
	_Send((char*)message.c_str(), client);
}

void Client::LeaveAllChannels()
{
	for(ChannelIterator iterator = this->_channels.begin(); iterator != this->_channels.end(); iterator++) 
	{
		channels[iterator->second]->leave(client);
	}
}

string Client::serializableObjectToString(map<string, SerializableObject> data)
{
	string result = "{";

	int counter = 0;
	for(std::map<string, SerializableObject>::iterator iterator = data.begin(); iterator != data.end(); iterator++) 
	{
		if(iterator->second.childrens.size()==0)
		{
			result+="\""+iterator->first+"\": \""+iterator->second.value+"\"";
		}
		else
		{
			result+="\""+iterator->first+"\": "+serializableObjectToString(iterator->second.childrens);
		}
		
		if(counter<data.size()-1)
			result+=",";

		counter++;
	}

	result += "}";

	return result;
}