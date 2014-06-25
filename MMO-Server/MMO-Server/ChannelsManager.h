#include "stdafx.h"
#include "Channel.h"

extern map<int, Channel*> channels;
extern map<string, int> channelsByName;

int channelIdCounter = 0;

Channel* addChannel(string name, int maxPlayers, bool autoLeave, bool isTemp)
{
	Channel* newChannel = new Channel(channelIdCounter, name, maxPlayers, autoLeave, isTemp);
	channels[newChannel->getId()] = newChannel;
	channelsByName[newChannel->getName()] = newChannel->getId();
	channelIdCounter++;

	return newChannel;
}

void destroyChannel(int id)
{
	Channel* channel = channels[id];
	channel->kickAll();
	channelsByName.erase(channel->getName());
	channels.erase(channel->getId());
}