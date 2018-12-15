#pragma once
#include <functional>
#include <ctime>
#include <cstring>
#include <map>

class TimedFunction
{
public:
	bool Enabled = true;
	bool Looped = false;
	float Delay = 0; //ms
	clock_t LastExecute = clock();
	std::string Descriptor = "";
	std::function<void(void* ptr)> Function = nullptr;

	std::string LastError = "";
	bool ThrewError = false;
};

static class TimingManager
{
public:
	static std::map<int, TimedFunction*> TimedFunctions;
	static void Update(void*);
	static int TimerCount;
	static int CreateTimer(std::function<void(void* ptr)>, float, bool = false, std::string = "");
	static void DeleteTimer(int);
	static void EnableTimer(int);
	static void DisableTimer(int);
	static void RestartTimer(int);
};