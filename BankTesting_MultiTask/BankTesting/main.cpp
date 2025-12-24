#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include "Bank.h"
#include "Accounts.h"

using namespace std;

// 랜덤 엔진 (난수 생성용)
void WorkerThread(Bank* bank, int count)
{
	// C++ 랜덤 라이브러리 설정
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<Int64> randAccount(1, 100); // 1~100번 계좌
	uniform_int_distribution<Int64> randAmount(10, 100); // 10~100원 이체

	for (int i = 0; i < count; ++i)
	{
		Int64 sender = randAccount(gen);
		Int64 receiver = randAccount(gen);
		Int64 money = randAmount(gen);

		// 자기 자신에게 이체하는 경우는 스킵
		if (sender == receiver) continue;

		// 이체 시도 (락이 제대로 안 걸려있다면 여기서 사고가 터짐)
		bank->Transfer(receiver, sender, money);
	}
}

int main()
{
	Bank kBank;
	const int ACCOUNT_COUNT = 1000;
	const int THREAD_COUNT = 100;
	const int OPS_PER_THREAD = 100000; // 스레드당 1만 번

	cout << "[System] 1. 계좌 " << ACCOUNT_COUNT << "개 생성 중..." << endl;
	for (int i = 0; i < ACCOUNT_COUNT; ++i)
	{
		Int64 id = kBank.CreateAccount(); // 기본 10,000원 지급된다고 가정
	}

	// 초기 자산 확인
	Int64 initialTotal = kBank.GetTotalAssets();
	cout << "[Check] 초기 은행 총 자산: " << initialTotal << "원" << endl;

	cout << "[System] 2. 멀티스레드 이체 전쟁 시작 (총 " << THREAD_COUNT * OPS_PER_THREAD << "건)" << endl;

	// 스레드 컨테이너
	vector<thread> threads;

	// 스레드 생성 및 실행
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.push_back(thread(WorkerThread, &kBank, OPS_PER_THREAD));
	}

	// 모든 스레드가 끝날 때까지 대기 (Join)
	for (auto& t : threads)
	{
		t.join();
	}

	cout << "[System] 3. 모든 거래 종료." << endl;

	// 최종 자산 확인
	Int64 finalTotal = kBank.GetTotalAssets();
	cout << "[Check] 최종 은행 총 자산: " << finalTotal << "원" << endl;

	// 검증
	if (initialTotal == finalTotal)
	{
		cout << ">>> [성공] 완벽합니다! 돈이 1원도 사라지지 않았습니다." << endl;
	}
	else
	{
		cout << ">>> [실패] 심각한 오류 발생!" << endl;
		cout << ">>> 사라지거나 복사된 돈: " << (finalTotal - initialTotal) << "원" << endl;
	}

	return 0;
}