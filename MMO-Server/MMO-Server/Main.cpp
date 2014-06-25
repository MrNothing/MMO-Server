// MMO-Server.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

#include "ChannelsManager.h"

#include "Core.h"
#include "Client.h"
#include "Channel.h"

typedef std::map<int, int>::iterator ChannelIterator;

map<int, Client*> clients;
map<string, int> loggedClientsByName;
map<int, Channel*> channels;
map<string, int> channelsByName;

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Loading config file...\n");
	
	string configJson = readFileAsString("config.json");
	
	rapidjson::Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	
	if (document.Parse<0>(configJson.c_str()).HasParseError())
	{
		cout<<"Could not load config file..."<<endl;
		while(true);
		return 1;
	}

	addChannel("Public 1", 100, true, false);
	addChannel("Chat 1", 100, false, false);

	start();
}

void OnClientMessage(SOCKET clientId, char* message)
{
	//first we parse the message, it has to be json, and it has to have a "type" key for the program to decide what to do

	rapidjson::Document data;
	if (data.Parse<0>(message).HasParseError())
	{
		//The client sent us non parsable json, we ignore him.
		//Send("{\"type\": \"err\"}", clientId);
		return;
	}

	string type;
	
	if(data.HasMember("type"))
		type = data["type"].GetString();
	else
	{
		type = "N/A";
	}
	
	if(!type.compare("ping"))
	{
		clients[clientId]->Send("ping");
	}
	else if (!type.compare("login"))
	{
		if(clients[clientId]->getName().length()==0)
		{
			if(data.HasMember("name"))
			{
				if(!exists<string, int>(loggedClientsByName, data["name"].GetString()))
				{
					//todo: datable password check + md5 encryption

					clients[clientId]->setName(data["name"].GetString());
					loggedClientsByName[clients[clientId]->getName()] = clientId;
				}
				else
				{
					clients[clientId]->Send("err", "msg", "this username is already taken!");
				}
			}
			else
			{
				clients[clientId]->Send("err", "msg", "no name specifed");
			}
		}
		else
		{
			clients[clientId]->Send("err", "msg", "already logged in!");
		}
	}
	else if(!type.compare("join"))
	{
		if(data.HasMember("id"))
		{
			if(exists<int, Channel*>(channels, data["id"].GetInt()))
				channels[data["id"].GetInt()]->join(clientId);
			else
				clients[clientId]->Send("err", "msg", "channel not found!");
		}
		else if(data.HasMember("name"))
		{
			if(exists<string, int>(channelsByName, data["name"].GetString()))
				channels[channelsByName[data["name"].GetString()]]->join(clientId);
			else
				clients[clientId]->Send("err", "msg", "channel not found!");
		}
		else
		{
			clients[clientId]->Send("err", "msg", "No channel specified!");
		}
	}
	else if(!type.compare("leave"))
	{
		if(data.HasMember("id"))
		{
			if(exists<int, Channel*>(channels, data["id"].GetInt()))
				channels[data["id"].GetInt()]->leave(clientId);
			else
				clients[clientId]->Send("err", "msg", "channel not found!");
		}
		else if(data.HasMember("name"))
		{
			if(exists<string, int>(channelsByName, data["name"].GetString()))
				channels[channelsByName[data["name"].GetString()]]->leave(clientId);
			else
				clients[clientId]->Send("err", "msg", "channel not found!");
		}
		else
		{
			clients[clientId]->LeaveAllChannels();
		}
	}
	else
	{
		//no such type
		clients[clientId]->Send("err", "msg", "no such type");
	}
}

void OnClientConnected(SOCKET clientId, sockaddr_in adress)
{
	cout<<"Connected: "<<clientId<<endl;
	Client* client = new Client(clientId, adress);
	clients[clientId] = client;
}

void OnClientDisconnected(SOCKET clientId, int reason)
{
	cout<<"Disconnected: "<<clientId<<" reason: "<<reason<<endl;

	if(clients[clientId]->getName().length()>0)
		loggedClientsByName.erase(clients[clientId]->getName());

	clients.erase(clientId);

	//leave every channel...
	map<int, int> clientChannels = clients[clientId]->getChannels();
	for(ChannelIterator iterator = clientChannels.begin(); iterator != clientChannels.end(); iterator++) 
	{
		channels[iterator->second]->leave(clientId);
	}
}

