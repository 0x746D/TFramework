#include "ConsoleManager.h"

void Console::WriteLine(const char* str)
{
	std::cout << str << std::endl;
}

void Console::WriteLine(std::string str)
{
	std::cout << str << std::endl;
}

void Console::Write(const char* str)
{
	std::cout << str;
}
void Console::Write(std::string str)
{
	std::cout << str;
}