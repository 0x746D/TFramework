#pragma once
#include <iostream>
#include <string>

static class Console
{
public:
	static void WriteLine(const char* str);
	static void WriteLine(std::string str);
	static void Write(const char* str);
	static void Write(std::string str);
};