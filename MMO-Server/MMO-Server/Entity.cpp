#pragma once

#include "stdafx.h"
#include "Entity.h"
#include "Channel.h"

extern Channel* GetChannel(int id);
typedef std::map<int, int>::iterator IntMapIterator;
typedef std::map<int, Entity*>::iterator EntityIterator;

Entity::Entity(void)
{
	owner = -1;
	lastPosId = string("");
}

Entity::~Entity()
{
	clearIndexes();
}

Entity::Entity(int _id, int _channel, string _name, Vector3 _position, float _moveSpeed)
{
	id=_id;
	channel=_channel;
	name=_name;
	position=_position;
	moveSpeed=_moveSpeed;

	owner = -1;
	lastPosId = string("");
}

void Entity::run()
{
	updateIndex();
}

void Entity::updateIndex()
{
	Channel* myChannel= GetChannel(channel);

	string posId = (position.flatten(myChannel->getIndexBlocSize())).toString();
	
	cout<<"posId "<<posId<<endl;
	
	//if i have moved over the bounds of my bloc
	if(!posId.compare(lastPosId)) 
	{
		//remove myself from last index...
		if(exists<int, int>(*myChannel->getSpacialEntityIndex()[lastPosId], id))
			myChannel->getSpacialEntityIndex()[lastPosId]->erase(id);

		if(owner>0)
		{
			if(exists<int, int>(*myChannel->getSpacialClientIndex()[lastPosId], id))
				myChannel->getSpacialClientIndex()[lastPosId]->erase(id);
		}

		
		//insert myself in the new index...
		(*myChannel->getSpacialEntityIndex()[posId])[id]=id;

		if(owner>0)
			(*myChannel->getSpacialClientIndex()[posId])[id]=id;
	}
}

void Entity::clearIndexes()
{
	Channel* myChannel=GetChannel(channel);

	if(exists<int, int>(*myChannel->getSpacialEntityIndex()[lastPosId], id))
		myChannel->getSpacialEntityIndex()[lastPosId]->erase(id);

	if(exists<int, int>(*myChannel->getSpacialClientIndex()[lastPosId], id))
		myChannel->getSpacialClientIndex()[lastPosId]->erase(id);
}

//if channel is has optimization flag on, this will only iterate on entities in range
void Entity::doSomethingWithAllVisibleEntities(function<void(int, SerializableObject)> action, SerializableObject params)
{
	Channel* myChannel=GetChannel(channel);

	if(myChannel->isOptimizingSync())
	{
		for(float i=-myChannel->getViewRange().x; i<=myChannel->getViewRange().x; i++)
		{
			for(float j=-myChannel->getViewRange().y; j<=myChannel->getViewRange().y; j++)
			{
				for(float k=-myChannel->getViewRange().z; k<=myChannel->getViewRange().z; k++)
				{
					string posId = (position.flatten(myChannel->getIndexBlocSize())+Vector3(i*myChannel->getIndexBlocSize(), j*myChannel->getIndexBlocSize(), k*myChannel->getIndexBlocSize())).toString();
					
					//if this bloc exists, iterate in it...
					if(exists<string, map<int, int>*>(myChannel->getSpacialEntityIndex(), posId))
					{
						//for each entity..
						for(IntMapIterator iterator = (*myChannel->getSpacialEntityIndex()[posId]).begin(); iterator != (*myChannel->getSpacialEntityIndex()[posId]).end(); iterator++) 
						{
							//do some custom stuff here...
							action(iterator->first, params);
						}
					}
				}
			}
		}
	}
	else
	{
		for(EntityIterator iterator = myChannel->getEntities().begin(); iterator != myChannel->getEntities().end(); iterator++) 
		{
			action(iterator->first, params);
		}
	}
}