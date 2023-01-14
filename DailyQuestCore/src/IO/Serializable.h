#pragma once

#include "Data.h"

class Serializable
{
public:
	virtual void Save(Data& data) = 0;
	virtual void Load(Data& data) = 0;
};