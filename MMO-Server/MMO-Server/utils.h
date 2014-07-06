#pragma once
#include "stdafx.h"
#include "Object.h"

using namespace std;

struct SerializableObject
{
	string value;
	map<string, SerializableObject> childrens;
	SerializableObject(void)
	{
		value="";
	}
	
	~SerializableObject()
	{
	
	}

	SerializableObject(string _value)
	{
		value = _value;
	}

	SerializableObject(int _value)
	{
		value = to_string(_value);
	}

	SerializableObject(float _value)
	{
		value = to_string(_value);
	}

	SerializableObject(double _value)
	{
		value = to_string(_value);
	}

	SerializableObject(bool _value)
	{
		value = to_string(_value);
	}

	SerializableObject(long _value)
	{
		value = to_string(_value);
	}

	SerializableObject(const char* _value)
	{
		value = _value;
	}

	SerializableObject(char _value)
	{
		value = to_string(_value);
	}

	SerializableObject(short _value)
	{
		value = to_string(_value);
	}

	SerializableObject(map<string, SerializableObject> _childrens)
	{
		childrens = _childrens;
		value="";
	}
};

template<typename T1, typename T2>bool exists(map<T1, T2> m, T1 key)
{
	if ( m.find(key) == m.end() ) {
	  return false;
	} else {
	  return true;
	}
}
