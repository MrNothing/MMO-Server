#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Vector3.h"
#include "Vector3i.h"

using namespace std;

template<typename T1, typename T2>bool exists(map<T1, T2> m, T1 key)
{
	if ( m.find(key) == m.end() ) {
	  return false;
	} else {
	  return true;
	}
}

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

	SerializableObject operator[](string index)
	{
		return childrens[index];
	}

	void operator=(map<string, SerializableObject> _childrens)
	{
		childrens = _childrens;
	}

	void operator=(string _value)
	{
		value = _value;
	}

	void operator=(int _value)
	{
		value = to_string(_value);
	}

	void operator=(float _value)
	{
		value = to_string(_value);
	}

	void operator=(char _value)
	{
		value = to_string(_value);
	}

	void operator=(long _value)
	{
		value = to_string(_value);
	}

	void operator=(bool _value)
	{
		value = to_string(_value);
	}

	void operator=(short _value)
	{
		value = to_string(_value);
	}

	void operator=(const char* _value)
	{
		value = _value;
	}

	//+ operator

	string operator+(string _value)
	{
		value += _value;
		return value;
	}

	int operator+(int _value)
	{
		value = to_string(atof(value.c_str())+_value);
		return atof(value.c_str());
	}

	float operator+(float _value)
	{
		value = to_string(atof(value.c_str())+_value);
		return atof(value.c_str());
	}

	char operator+(char _value)
	{
		value = to_string(atof(value.c_str())+_value);
		return atof(value.c_str());
	}

	long operator+(long _value)
	{
		value = to_string(atof(value.c_str())+_value);
		return atof(value.c_str());
	}

	short operator+(short _value)
	{
		value = to_string(atof(value.c_str())+_value);
		return atof(value.c_str());
	}

	const char* operator+(const char* _value)
	{
		value += _value;
		return value.c_str();
	}

	//- operator

	void operator-(string _value)
	{
		value = _value;
	}

	int operator-(int _value)
	{
		value = to_string(atof(value.c_str())-_value);
		return atof(value.c_str());
	}

	float operator-(float _value)
	{
		value = to_string(atof(value.c_str())-_value);
		return atof(value.c_str());
	}

	char operator-(char _value)
	{
		value = to_string(atof(value.c_str())-_value);
		return atof(value.c_str());
	}

	long operator-(long _value)
	{
		value = to_string(atof(value.c_str())-_value);
		return atof(value.c_str());
	}

	short operator-(short _value)
	{
		value = to_string(atof(value.c_str())-_value);
		return atof(value.c_str());
	}

	void operator-(const char* _value)
	{
		value = _value;
	}

	//* operator

	void operator*(string _value)
	{
		value = _value;
	}

	int operator*(int _value)
	{
		value = to_string(atof(value.c_str())*_value);
		return atof(value.c_str());
	}

	float operator*(float _value)
	{
		value = to_string(atof(value.c_str())*_value);
		return atof(value.c_str());
	}

	char operator*(char _value)
	{
		value = to_string(atof(value.c_str())*_value);
		return atof(value.c_str());
	}

	long operator*(long _value)
	{
		value = to_string(atof(value.c_str())*_value);
		return atof(value.c_str());
	}

	short operator*(short _value)
	{
		value = to_string(atof(value.c_str())*_value);
		return atof(value.c_str());
	}

	void operator*(const char* _value)
	{
		value = _value;
	}

	// / operator

	void operator/(string _value)
	{
		value = _value;
	}

	int operator/(int _value)
	{
		value = to_string(atof(value.c_str())/_value);
		return atof(value.c_str());
	}

	float operator/(float _value)
	{
		value = to_string(atof(value.c_str())/_value);
		return atof(value.c_str());
	}

	char operator/(char _value)
	{
		value = to_string(atof(value.c_str())/_value);
		return atof(value.c_str());
	}

	long operator/(long _value)
	{
		value = to_string(atof(value.c_str())/_value);
		return atof(value.c_str());
	}

	short operator/(short _value)
	{
		value = to_string(atof(value.c_str())/_value);
		return atof(value.c_str());
	}

	void operator/(const char* _value)
	{
		value = _value;
	}

	// ==

	bool operator==(string _value)
	{
		return !value.compare(_value);
	}

	bool operator==(const char* _value)
	{
		return !value.compare(_value);
	}

	bool operator==(int _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(float _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(bool _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(double _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(long _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(short _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator==(SerializableObject _value)
	{
		return !value.compare(_value.value);
	}

	// !=

	bool operator!=(string _value)
	{
		return value.compare(_value);
	}

	bool operator!=(const char* _value)
	{
		return value.compare(_value);
	}

	bool operator!=(int _value)
	{
		return !value.compare(to_string(_value));
	}

	bool operator!=(float _value)
	{
		return value.compare(to_string(_value));
	}

	bool operator!=(bool _value)
	{
		return value.compare(to_string(_value));
	}

	bool operator!=(double _value)
	{
		return value.compare(to_string(_value));
	}

	bool operator!=(long _value)
	{
		return value.compare(to_string(_value));
	}

	bool operator!=(short _value)
	{
		return value.compare(to_string(_value));
	}

	bool operator!=(SerializableObject _value)
	{
		return value.compare(_value.value);
	}

	string GetString(){return value;}
	int GetInt(){return atoi(value.c_str());}
	long GetLong(){return atoi(value.c_str());}
	short GetShort(){return atoi(value.c_str());}
	float GetFloat(){return atof(value.c_str());}
	double GetDouble(){return atof(value.c_str());}
	bool GetBool(){return atoi(value.c_str());}
	char GetChar(){return atoi(value.c_str());}
	const char* GetC_String(){return value.c_str();}

	void Add(string index, string value){childrens[index]=SerializableObject(value);}
	void Add(string index, int value){childrens[index]=SerializableObject(value);}
	void Add(string index, long value){childrens[index]=SerializableObject(value);}
	void Add(string index, short value){childrens[index]=SerializableObject(value);}
	void Add(string index, float value){childrens[index]=SerializableObject(value);}
	void Add(string index, double value){childrens[index]=SerializableObject(value);}
	void Add(string index, bool value){childrens[index]=SerializableObject(value);}
	void Add(string index, char value){childrens[index]=SerializableObject(value);}
	void Add(string index, const char* value){childrens[index]=SerializableObject(value);}
	void Add(string index, SerializableObject value){childrens[index]=value;}

	bool contains(string index){ return exists<string, SerializableObject>(childrens, index);}
};
