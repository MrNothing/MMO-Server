#include "stdafx.h"
#include "ExtensionBase.h"
#include "Entity.h"

using namespace std;

void Start()
{
	//Do something here...
}

void showWhoISee(int entity, SerializableObject params)
{
	cout<<"I see this entity: "<<entity<<endl;
}

void Update()
{
	//Do something here...
	GetChannel(2)->getEntities()[0]->doSomethingWithAllVisibleEntities(showWhoISee, SerializableObject(""));
}

void onCustomMessageRecieved(Client client, SerializableObject data)
{
	//Do something here...
}

void onClientLogin(Client client)
{
	//Do something here...
}

void onClientLogout(Client client)
{
	//Do something here...
}

void onClientDisconnected(Client client)
{
	//Do something here...
}