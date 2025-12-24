#pragma once
#include <thread>
#include <mutex>


class Lock
{
public:

	std::mutex m;

	Lock();
	~Lock();

	Lock(const Lock&) = delete;
	Lock& operator=(const Lock&) = delete;

	void WriteLock();
	void WriteUnLock();
};

