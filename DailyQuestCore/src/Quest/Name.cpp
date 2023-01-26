#include "Name.h"

int Name::_count = 0;


std::shared_ptr<Name> Name::Create(std::string value)
{
	return std::make_shared<Name>(value);
}