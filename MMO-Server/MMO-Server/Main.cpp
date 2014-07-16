// MMO-Server.cpp : définit le point d'entrée pour l'application console.
//
#pragma once

#include "stdafx.h"
#include "utils.h"

#include "ChannelsManager.h"
#include "ChatManager.h"

#include "Core.h"
#include "Client.h"
#include "Channel.h"

#include "ExtensionBase.h"

typedef std::map<int, int>::iterator ChannelIterator;
typedef std::map<int, Channel*>::iterator ChannelMapIterator;

map<int, Client*> clients;
map<string, int> loggedClientsByName;
map<int, Channel*> channels;
map<string, int> channelsByName;
map<int, int> publicChannels;

//server config
int maxClients = 9999;
string serverIp = "127.0.0.1";
int serverPort = 6600;
bool unsecurePublicData = true;
bool unsecurePersistentData = true;

unsigned int updateInterval=100;

void run();

int _tmain(int argc, _TCHAR* argv[])
{

	Log("Loading config file...");
	
	string configJson = readFileAsString("config.json");
	
	rapidjson::Document config;
	
	if (config.Parse<0>(configJson.c_str()).HasParseError())
	{
		Log("Could not parse config file...");
		while(true);
		return 1;
	}
	
	if(config.HasMember("enable client side public data"))
		unsecurePublicData = config["enable client side public data"].GetBool();

	if(config.HasMember("enable client side persistent data"))
		unsecurePersistentData = config["enable client side public data"].GetBool();

	if(config.HasMember("serverIp"))
		serverIp = config["serverIp"].GetString();

	if(config.HasMember("serverPort"))
	{
		if(config["serverPort"].IsInt())
			serverPort = config["serverPort"].GetInt();
		else
			Log("ServerPort was not in Int format, please specify a number. Default value will be used.");
	}

	if(config.HasMember("maxUsers"))
	{
		if(config["maxUsers"].IsInt())
			maxClients = config["maxUsers"].GetInt();
		else
			Log("maxUsers was not in Int format, please specify a number. Default value will be used.");
	}

	if(config.HasMember("channels"))
	{
		rapidjson::Value& channels = config["channels"];

		if(channels.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < channels.Size(); i++)
			{
				rapidjson::Value& channel = channels[i];
				Log(string("Channel found: ")+channel["name"].GetString());
				addChannel(channel["name"].GetString(), channel["maxUsers"].GetInt(), channel["autoLeave"].GetBool(), channel["isTemp"].GetBool(), channel["isPublic"].GetBool());
			}
		}
	}

	Log("found "+to_string(channels.size())+" channels");

	timer_start(run, updateInterval);
	
	start();
}

void run()
{
	for(ChannelMapIterator iterator = channels.begin(); iterator != channels.end(); iterator++) 
	{
		iterator->second->run();
	}

	Update();
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
	{
		if(data["type"].IsString())
			type = data["type"].GetString();
		else
			type = "N/A";
	}
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
				if(data["name"].IsString())
				{
					string name = data["name"].GetString();
					if(name.length()>0)
					{
						if(!exists<string, int>(loggedClientsByName, data["name"].GetString()))
						{
							//todo: database password check + md5 encryption

							clients[clientId]->setName(data["name"].GetString());
							loggedClientsByName[clients[clientId]->getName()] = clientId;

							map<string, string> data;
							data["type"] = "login";
							data["id"] = to_string(clientId);
							data["name"] = clients[clientId]->getName();
							clients[clientId]->Send(data);

							onClientLogin(*clients[clientId]);
						}
						else
						{
							clients[clientId]->Send("err", "id", "1"); //this username is already taken!
						}
					}
					else
						clients[clientId]->Send("err", "id", "0p"); //parsing error
				}
				else
					clients[clientId]->Send("err", "id", "11"); //username cannot be empty!
			}
			else
			{
				clients[clientId]->Send("err", "id", "2"); //no name specifed
			}
		}
		else
		{
			clients[clientId]->Send("err", "id", "3"); //already logged in!
		}
	}
	else if (!type.compare("logout"))
	{
		if(clients[clientId]->getName().length()!=0)
		{
			//leave every channel...
			map<int, int> clientChannels = clients[clientId]->getChannels();
			for(ChannelIterator iterator = clientChannels.begin(); iterator != clientChannels.end(); iterator++) 
			{
				channels[iterator->second]->leave(clientId);
			}

			loggedClientsByName.erase(clients[clientId]->getName());

			onClientLogout(*clients[clientId]);

			clients[clientId]->Send("logout", "msg", "userTriggered");
		}
		else
			clients[clientId]->Send("err", "id", "10"); //you are not logged in!
	}
	else if(!type.compare("join"))
	{
		if(data.HasMember("id"))
		{
			if(data["id"].IsInt())
			{
				if(exists<int, Channel*>(channels, data["id"].GetInt()))
					channels[data["id"].GetInt()]->join(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else if(data.HasMember("name"))
		{
			if(data["name"].IsString())
			{
				if(exists<string, int>(channelsByName, data["name"].GetString()))
					channels[channelsByName[data["name"].GetString()]]->join(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else
		{
			clients[clientId]->Send("err", "id", "5"); //No channel specified!
		}
	}
	else if(!type.compare("leave"))
	{
		if(data.HasMember("id"))
		{
			if(data["id"].IsInt())
			{
				if(exists<int, Channel*>(channels, data["id"].GetInt()))
					channels[data["id"].GetInt()]->leave(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else if(data.HasMember("name"))
		{
			if(data["name"].IsString())
			{
				if(exists<string, int>(channelsByName, data["name"].GetString()))
					channels[channelsByName[data["name"].GetString()]]->leave(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else
		{
			clients[clientId]->LeaveAllChannels();
		}
	}
	else if(!type.compare("getUsers"))
	{
		if(data.HasMember("id"))
		{
			if(data["id"].IsInt())
			{
				if(exists<int, Channel*>(channels, data["id"].GetInt()))
					channels[data["id"].GetInt()]->SendUsers(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else if(data.HasMember("name"))
		{
			if(data["name"].IsString())
			{
				if(exists<string, int>(channelsByName, data["name"].GetString()))
					channels[channelsByName[data["name"].GetString()]]->SendUsers(clientId);
				else
					clients[clientId]->Send("err", "id", "4"); //channel not found!
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else
		{
			clients[clientId]->Send("err", "id", "5"); //No channel specified!
		}
	}
	else if(!type.compare("getChannels"))
	{
		if(clients[clientId]->getName().length()!=0)
		{
			map<string, SerializableObject> data;
			data["type"] = SerializableObject(string("channels"));
			
			cout<<"type is: "<<data["type"].value<<endl;

			map<string, SerializableObject> serializedChannels;
			for(ChannelIterator iterator = publicChannels.begin(); iterator != publicChannels.end(); iterator++) 
			{
				if(channels[iterator->second]->isPublic())
				{
					map<string, SerializableObject> channelObj;
					channelObj["id"] = SerializableObject(channels[iterator->second]->getId());
					channelObj["name"] = SerializableObject(channels[iterator->second]->getName());
					channelObj["maxUsers"] = SerializableObject(channels[iterator->second]->getMaxClients());
					channelObj["users"] = SerializableObject(channels[iterator->second]->getClientsCount());
					serializedChannels[to_string(channels[iterator->second]->getId())] = channelObj;
				}
			}

			data["channels"] = SerializableObject(serializedChannels);

			clients[clientId]->Send(data);
		}
		else
			clients[clientId]->Send("err", "id", "10"); //you are not logged in!
	}
	else if(!type.compare("chat")) //handle chat commands here
	{
		if(clients[clientId]->getChannels().size()==0)
		{
			clients[clientId]->Send("err", "id", "6"); //You are not in a channel!
			return;
		}

		if(data.HasMember("action"))
		{
			if(data["action"].IsString())
			{
				if(data.HasMember("msg"))
				{
					string channel = "";
					if(data.HasMember("channel"))
					{
						if(data["channel"].IsString())
							channel = data["channel"].GetString();
					}

					if(data["msg"].IsString())
						handleChatAction(clients[clientId], data["action"].GetString(), data["msg"].GetString(), channel, "");
					else
						clients[clientId]->Send("err", "id", "0p"); //parsing error
				}
				else
				{
					string channel = "";
					if(data.HasMember("channel"))
					{
						if(data["channel"].IsString())
							channel = data["channel"].GetString();
					}

					string to = "";
					if(data.HasMember("to"))
					{
						if(data["to"].IsString())
							to = data["to"].GetString();
					}

					if(data["msg"].IsString())
						handleChatAction(clients[clientId], data["action"].GetString(), data["msg"].GetString(), channel, to);
					else
						clients[clientId]->Send("err", "id", "0p"); //parsing error
				}
			}
			else
				clients[clientId]->Send("err", "id", "0p"); //parsing error
		}
		else
		{
			//message was in wrong format... ignore
			clients[clientId]->Send("err", "id", "0c"); //no chat action
		}
	}
	else if(!type.compare("_d") && unsecurePublicData) //set public data
	{
		if(data.HasMember("n") && data.HasMember("d"))
		{
			if(data["n"].IsString())
			{
				SerializableObject parsedData = parseRapidJson(data["d"]);
				clients[clientId]->SetPublicData(data["n"].GetString(), parsedData);
			}
		}
	}
	else if(!type.compare("_p") && unsecurePersistentData) //set persistent data
	{
		if(data.HasMember("n") && data.HasMember("d"))
		{
			if(data["n"].IsString())
			{
				SerializableObject parsedData = parseRapidJson(data["d"]);
				clients[clientId]->SetPersistentData(data["n"].GetString(), parsedData);
			}
		}
	}
	else if(!type.compare("_gp")) //get persistent data
	{
		if(data.HasMember("name"))
		{
			if(data["name"].IsString())
			{
				clients[clientId]->SendPersistentData(data["name"].GetString());
			}
		}
	}
	else if(!type.compare("_p_all")) //get all persistent data
	{
		clients[clientId]->SendAllPersistentData();
	}
	else if(!type.compare("_cu")) //custom message
	{
		if(data.HasMember("d"))
		{
			SerializableObject parsedData = parseRapidJson(data["d"]);
			onCustomMessageRecieved(*clients[clientId], parsedData);
		}
	}
	else
	{
		//no such type
		clients[clientId]->Send("err", "id", "0"); //no such type
	}
}

void OnClientConnected(SOCKET clientId, sockaddr_in adress)
{
	Log("Connected: "+to_string(clientId));
	Client* client = new Client(clientId, adress);
	clients[clientId] = client;
}

void OnClientDisconnected(SOCKET clientId, int reason)
{
	Log("Disconnected: "+to_string(clientId)+" reason: "+to_string(reason));
	
	clients[clientId]->setDisconnected(true);

	if(exists<int, Client*>(clients, clientId))
	{
		//leave every channel...
		map<int, int> clientChannels = clients[clientId]->getChannels();
		for(ChannelIterator iterator = clientChannels.begin(); iterator != clientChannels.end(); iterator++) 
		{
			channels[iterator->second]->leave(clientId);
		}

		if(clients[clientId]->getName().length()>0)
		{
			//save persistent data...
			writeString(clients[clientId]->serializableObjectToString(clients[clientId]->getPersistentData()), "Persistent/"+clients[clientId]->getName()+"_data.json");

			loggedClientsByName.erase(clients[clientId]->getName());
			onClientDisconnected(*clients[clientId]);
		}
		clients.erase(clientId);

		delete[] clients[clientId];
	}
}

bool GetChannel(int id, Channel* channel)
{
	if(exists<int, Channel*>(channels, id))
	{
		channel = channels[id];
		return true;
	}
	else
	{
		return false;
	}
}
