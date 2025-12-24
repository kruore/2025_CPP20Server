#include "Bank.h"
#include "Accounts.h"

Bank::Bank() : accountsNum(0)
{
}

Bank::~Bank()
{
	data.clear(); // 스마트 포인터라 자동 해제되지만 명시적으로 비움
}
Int64 Bank::GetTotalAssets()
{
	Int64 total = 0;
	for (auto& pair : data)
	{
		total += pair.second->GetBalance();
	}
	return total;
}

Int64 Bank::CreateAccount()
{
	Int64 newId = accountsNum.fetch_add(1);

	std::shared_ptr<Accounts> newAccount = std::make_shared<Accounts>(newId, 10000);

	data.insert({ newId, newAccount });

	return newId;
}

bool Bank::DeleteAccount(Int64 number)
{
	auto it = data.find(number);
	if (it != data.end())
	{
		data.erase(it);
		return true;
	}
	return false;
}

bool Bank::Transfer(Int64 recvId, Int64 sendId, Int64 amount)
{
	auto itRecv = data.find(recvId);
	auto itSend = data.find(sendId);

	if (itRecv == data.end() || itSend == data.end())
		return false;

	auto recvAcc = itRecv->second;
	auto sendAcc = itSend->second;

	if (sendAcc->Withdraw(amount))
	{
		recvAcc->Deposit(amount);
		return true;
	}

	return false;
}

Int64 Bank::GetBalance(Int64 id)
{
	auto it = data.find(id);
	if (it != data.end())
	{
		return it->second->GetBalance();
	}
	return -1;
}