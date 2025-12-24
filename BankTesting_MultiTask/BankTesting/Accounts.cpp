#include "Accounts.h"
#include <atomic>

Accounts::Accounts(Int64 id, Int64 balance) : _id(id), _balance(balance)
{

}
Accounts::~Accounts()
{
}


void Accounts::Deposit(Int64 amount)
{
	_balance += amount;
}

bool Accounts::Withdraw(Int64 amount)
{
	if (_balance - amount > 0)
	{
		_balance -= amount;
		return true;
	}
	return false;

}

Int64 Accounts::GetBalance() const
{
	return _balance;
}