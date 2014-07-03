#pragma once

#include "stdafx.h"
#include "Channel.h"

extern map<int, Channel*> channels;
extern map<string, int> channelsByName;
extern map<int, int> publicChannels;

int channelIdCounter = 0;

Channel* addChannel(string name, int maxPlayers, bool autoLeave, bool isTemp, bool isPublic)
{
	Channel* newChannel = new Channel(channelIdCounter, name, maxPlayers, autoLeave, isTemp, isPublic);
	channels[newChannel->getId()] = newChannel;
	channelsByName[newChannel->getName()] = newChannel->getId();

	if(newChannel->isPublic())
		publicChannels[newChannel->getId()] = newChannel->getId();

	channelIdCounter++;

	return newChannel;
}

void destroyChannel(int id)
{
	Channel* channel = channels[id];
	channel->kickAll();
	channelsByName.erase(channel->getName());
	channels.erase(channel->getId());
	if(channel->isPublic())
		publicChannels.erase(channel->getId());

	delete[] channel;
}