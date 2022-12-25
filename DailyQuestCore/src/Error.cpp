#include "Error.h"

void Error::fatalError(std::string text)
{
	std::cout << "Fatal error : " << text << std::endl;
	Application::Stop();
}

void Error::warning(std::string text)
{
	std::cout << "Warning : " << text << std::endl;
}
