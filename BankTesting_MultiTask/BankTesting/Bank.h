#pragma once
#include "Types.h"
#include <unordered_map>
#include <memory> // shared_ptr 필수

class Accounts; // 전방 선언

class Bank
{
public:
	Bank();
	~Bank();

	Int64 GetTotalAssets();

	Int64 CreateAccount();
	bool DeleteAccount(Int64 number);
	bool Transfer(Int64 recv, Int64 send, Int64 amount);
	Int64 GetBalance(Int64 id);

private:
	std::unordered_map<Int64, std::shared_ptr<Accounts>> data;
	std::atomic<Int64> accountsNum;
};