#include "TimingManager.h"

std::map<int, TimedFunction*> TimingManager::TimedFunctions;

void TimingManager::Update(void* ptr)
{
	std::map<int, TimedFunction*>::iterator it;
	for (it = TimedFunctions.begin(); it != TimedFunctions.end(); it++)
	{
		if (it->second->Enabled && !it->second->ThrewError)
		{
			clock_t clk = clock(); //To avoid time loss in function execution (1)
			if (float(clk - it->second->LastExecute) >= it->second->Delay)
			{
				try
				{
					it->second->Function(ptr);
					it->second->LastError = "";
					it->second->ThrewError = false;
				}
				catch(const std::runtime_error& re)
				{
					it->second->LastError = re.what();
					it->second->ThrewError = true;
					it->second->Enabled = false;
				}
				catch (const std::exception& ex)
				{
					it->second->LastError = ex.what();
					it->second->ThrewError = true;
					it->second->Enabled = false;
				}
				catch (...)
				{
					it->second->LastError = "Unknown try-catch error";
					it->second->ThrewError = true;
					it->second->Enabled = false;
				}

				it->second->LastExecute = clk; //(1)
				if (!it->second->Looped)
				{
					TimedFunctions.erase(it->first);
				}
			}
		}
	}
}

int TimingManager::TimerCount = 0;

int TimingManager::CreateTimer(std::function<void(void* ptr)> f, float delay, bool loop, std::string desc)
{
	TimedFunction* func = new TimedFunction();
	func->Delay = delay;
	func->Function = f;
	func->Looped = loop;
	func->Descriptor = desc;
	TimedFunctions.emplace(TimerCount, func);
	return TimerCount++;
}

void TimingManager::DeleteTimer(int id)
{
	std::map<int, TimedFunction*>::iterator it;
	for (it = TimedFunctions.begin(); it != TimedFunctions.end(); it++)
	{
		if (it->first == id)
		{
			TimedFunctions.erase(id);
			return;
		}
	}
}

void TimingManager::EnableTimer(int id)
{
	std::map<int, TimedFunction*>::iterator it;
	for (it = TimedFunctions.begin(); it != TimedFunctions.end(); it++)
	{
		if (it->first == id)
		{
			it->second->Enabled = true;
			return;
		}
	}
}

void TimingManager::DisableTimer(int id)
{
	std::map<int, TimedFunction*>::iterator it;
	for (it = TimedFunctions.begin(); it != TimedFunctions.end(); it++)
	{
		if (it->first == id)
		{
			it->second->Enabled = false;
			return;
		}
	}
}
void TimingManager::RestartTimer(int id)
{
	std::map<int, TimedFunction*>::iterator it;
	for (it = TimedFunctions.begin(); it != TimedFunctions.end(); it++)
	{
		if (it->first == id)
		{
			it->second->LastExecute = clock();
			return;
		}
	}
}