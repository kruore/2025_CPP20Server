#pragma once
#include "Types.h"
#include <mutex>


class Accounts
{
public:
	Accounts() = delete;
	Accounts(Int64 id, Int64 balance);
	~Accounts();

	void Deposit(Int64 amount);
	bool Withdraw(Int64 amount);

	Int64 GetBalance() const;
	Int64 GetID() const { return _id; }

	std::mutex& GetMutex() { return _mutex; }

private:
	Int64 _id;
	Int64 _balance;
	mutable std::mutex _mutex;
};