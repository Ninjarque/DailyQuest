#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <iostream>
#include <fstream>

#include "Data.h"
#include "Serializable.h"

class ContentManager
{
public:
	static void Init();

	static bool Save(std::string file, Data& data);
	static Data* Load(std::string file);
	static Data* Load(std::string file, Data* dataToFill);

	static bool Save(std::string file, Serializable* data);
	template<class T> static T* Load(std::string file)
	{
		Data* d = new Data();
		T* t = Load<T>(file, d);
		delete d;
		return t;
	}
	template<class T> static T* Load(std::string file, Data* dataToFill, bool ignoreRoot = true)
	{
		int hierarchy = 0;
		Data* realData = dataToFill;
		while (ignoreRoot && realData->GetChildCount() == 1)
		{
			realData = realData->GetChild(0);
		}
		if (!Data::Read(*dataToFill, file))
			return nullptr;

		if (ignoreRoot)
		{
			dataToFill = realData;
		}

		Serializable* s = (Serializable*)(new T());
		s->Load(*dataToFill);

		return (T*)s;
	}
private:
	static std::string Root;
};

