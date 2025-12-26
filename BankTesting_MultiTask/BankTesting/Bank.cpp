#include "Bank.h"
#include "Accounts.h"

Bank::Bank()
{
}

Bank::~Bank()
{
	datas.clear();
}

Int64 Bank::GetTotalAssets()
{
	return Int64();
}

Int64 Bank::CreateAccount()
{
	Int64 number = accountsNum.fetch_add(1);
	shared_ptr<Accounts> account = make_shared<Accounts>(number,10000);
	datas.insert({number,account});
	return number;
}

bool Bank::DeleteAccount(Int64 number)
{
	auto it = datas.find(number);
	if (it != datas.end())
	{
		datas.erase(it);
			return true;
	}
	return false;
}

bool Bank::Transfer(Int64 recv, Int64 send, Int64 amount)
{
	auto itRecv = datas.find(recv);
	auto itSend = datas.find(send);

	if (itRecv == datas.end() || itSend == datas.end())
	{
		return false;
	}

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
	auto it = datas.find(id);
	if (it != datas.end())
	{
		return it->second->GetBalance();
	}
	return -1;
}
