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

//Send message to all clients in all joined channels.
void Client::BroadCast(map<string, SerializableObject> data)
{
	if(hasDisconnected())
		return;

	for(ChannelIterator iterator = _channels.begin(); iterator != _channels.end(); iterator++) 
	{
		channels[iterator->second]->Send(data);
	}

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

void Client::SendPersistentData(string name)
{
	map<string, SerializableObject> data;
	data["type"] = SerializableObject(string("_p"));
	data["name"] = SerializableObject(name);
	data["data"] = SerializableObject(persistentData["name"]);
	Send(data);
}

void Client::SendAllPersistentData()
{
	map<string, SerializableObject> data;
	data["type"] = SerializableObject(string("_p_all"));
	data["data"] = SerializableObject(persistentData);
	Send(data);
}

void Client::SetPublicData(string name, SerializableObject _data)
{
	publicData[name] = _data;

	map<string, SerializableObject> data;
	data["type"] = SerializableObject(string("_d"));
	data["u"] = SerializableObject(getId());
	data["n"] = SerializableObject(name);
	data["d"] = SerializableObject(_data);
	BroadCast(data);
}

void Client::SendAllPublicData()
{
	map<string, SerializableObject> data;
	data["type"] = SerializableObject(string("_d_all"));
	data["u"] = SerializableObject(getId());
	data["d"] = SerializableObject(publicData);
	BroadCast(data);
}

void Client::SetPersistentData(string name, SerializableObject data)
{
	persistentData[name] = data;
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

template<typename T>string Client::ObjectToString(Object<T> data)
{
	string result = "{";

	if(data.size()==0)
	{
		result+="\"o\":"data.getValue();
	}
	else
	{
		map<string, T> childrens = data.getChildren();
		int counter = 0;
		for(std::map<string, T>::iterator iterator = childrens.begin(); iterator != childrens.end(); iterator++) 
		{
			if(iterator->second.getChildren().size()==0)
			{
				result+="\""+iterator->first+"\": \""+iterator->second.getValue()+"\"";
			}
			else
			{
				result+="\""+iterator->first+"\": "+ObjectToString(iterator->second);
			}
		
			if(counter<data.size()-1)
				result+=",";

			counter++;
		}

		result += "}";

		return result;
	}
}