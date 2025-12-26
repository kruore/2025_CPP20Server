// C++17 이상 필요
struct Account {
	int id;
	long long balance;
	mutable std::shared_mutex m; // 읽기/쓰기 락 분리

	Account(int _id, long long _bal) : id(_id), balance(_bal) {}

	// 입금 (쓰기 락)
	void Deposit(long long amount) {
		std::unique_lock<std::shared_mutex> lock(m);
		balance += amount;
	}

	// 출금 (쓰기 락)
	bool Withdraw(long long amount) {
		std::unique_lock<std::shared_mutex> lock(m);
		if (balance < amount) return false;
		balance -= amount;
		return true;
	}

	// 잔액 조회 (읽기 락 - 여러 스레드 동시 접근 가능)
	long long GetBalance() const {
		std::shared_lock<std::shared_mutex> lock(m);
		return balance;
	}
};