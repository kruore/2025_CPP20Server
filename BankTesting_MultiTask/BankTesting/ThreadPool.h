#pragma once
#include "Types.h"

class Bank;

class ThreadPool
{
public:
	ThreadPool(size_t numThreads);
	~ThreadPool();

	// 일감(함수)을 큐에 넣는 함수
	void EnqueueJob(std::function<void()> job);
private:
	// 스레드들이 계속 돌면서 실행할 함수
	void WorkerThread();

private:
	std::vector<std::thread> _workers;          // 일꾼들 (스레드)
	std::queue<std::function<void()>> _jobs;    // 일감 (큐)

	std::mutex _m;                              // 동기화 객체
	std::condition_variable _cv;                // 스레드 대기/신호 객체
	bool _stop = false;                         // 종료 플래그
};