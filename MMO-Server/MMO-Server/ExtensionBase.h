#pragma once;

#include "stdafx.h"
#include "Client.h"

void onCustomMessageRecieved(Client client, SerializableObject data);
void onClientLogin(Client client);
void onClientLogout(Client client);
void onClientDisconnected(Client client);
void Start();
void Update();
