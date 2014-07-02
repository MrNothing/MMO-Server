#pragma once

#include "stdafx.h"
#include "Channel.h"
#include "Client.h"

using namespace std;

extern map<int, Client*> clients;
extern map<string, int> loggedClientsByName;
extern map<int, Channel*> channels;
extern map<string, int> channelsByName;

void handleChatAction(Client* sender, string action, string message, string channel="", string to="")
{
	if(!action.compare("msg")) //send message to everyone in the specified channel
	{
		Channel* chosenChannel;
		if(channel.length()==0) //if no channel was specified, pick the first one in the list of joined channels
			chosenChannel = channels[sender->getChannels()[0]];
		else
		{
			if(exists<string, int>(channelsByName, channel))
			{
				chosenChannel = channels[channelsByName[channel]];
			}
			else
			{
				sender->Send("err", "id", "4"); //channel not found!
				return;
			}
		}

		map<string, string> data;
		data["type"] = "msg";
		data["msg"] = message;
		data["a_id"] = to_string(sender->getId());
		data["a_n"] = sender->getName();
		data["c_id"] = to_string(chosenChannel->getId());
		data["c_n"] = chosenChannel->getName();

		chosenChannel->Send(data);
	}
	else if(!action.compare("pm")) //send a private message
	{
		if(exists<string, int>(loggedClientsByName, to))
		{
			map<string, string> data;
			data["type"] = "pm";
			data["msg"] = message;
			data["a_id"] = to_string(sender->getId());
			data["a_n"] = sender->getName();
		}
		else
		{
			sender->Send("err", "id", "7"); //user not found!
		}
	}
	else
	{
		//no such action...
		sender->Send("err", "id", "0a"); //no such action
	}
}