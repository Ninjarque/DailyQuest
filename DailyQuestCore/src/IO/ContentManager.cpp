#include "ContentManager.h"

std::string ContentManager::Root;

void ContentManager::Init(std::string root)
{
	Root = root;
}

bool ContentManager::Save(std::string file, Data& data)
{
	return Data::Write(data, file);
}

Data* ContentManager::Load(std::string file)
{
	Data* d = new Data();

	return Load(file, d);
}

Data* ContentManager::Load(std::string file, Data* dataToFill)
{
	Data::Read(*dataToFill, file);

	return dataToFill;
}


bool ContentManager::Save(std::string file, Serializable* data)
{
	Data d;
	data->Save(d);
	return Data::Write(d, file);
}