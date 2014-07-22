#pragma once;

#include "stdafx.h"
#include "Client.h"
#include "Channel.h"

extern Channel* GetChannel(int id);

void onCustomMessageRecieved(Client client, SerializableObject data);
void onClientLogin(Client client);
void onClientLogout(Client client);
void onClientDisconnected(Client client);
void Start();
void Update();
