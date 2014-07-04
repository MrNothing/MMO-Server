#pragma once
#include "stdafx.h"

using namespace std;

template<typename T>class Object
{
public:
	Object<T>(void){ isNull=true;}
	Object<T>(T val){ value = val; isNull=false;}
	~Object<T>(void){}
	bool operator==(T param){ return T==value;}
	void operator+(T param){ value+=param}
	void operator-(T param){ value-=param}
	void operator*(T param){ value*=param}
	void operator/(T param){ value/=param}
	T operator[](string param){ return childrens[param];}
	int size(){ return childrens.size();}
	T getValue(){ return value;}
	map<string, T> getChildren(){ return childrens;}
private :
	bool isNull;
	T value;
	map<string, T> childrens;
};
