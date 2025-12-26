#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
    // 1. 지정된 수만큼 스레드(일꾼)를 미리 생성
    for (size_t i = 0; i < numThreads; ++i)
    {
        _workers.emplace_back([this]() { this->WorkerThread(); });
    }
}

ThreadPool::~ThreadPool()
{
    // 1. 종료 신호를 보냄
    {
        std::unique_lock<std::mutex> lock(_m);
        _stop = true;
    }
    // 2. 자고 있는 모든 스레드를 깨움
    _cv.notify_all();

    // 3. 모든 스레드가 일을 끝내고 죽을 때까지 기다림
    for (auto& worker : _workers)
    {
        if (worker.joinable())
            worker.join();
    }
}

void ThreadPool::EnqueueJob(std::function<void()> job)
{
    {
        std::lock_guard<std::mutex> lock(_m);
        _jobs.push(job); // 큐에 일감 넣기
    }
    // 자고 있는 스레드 중 하나를 깨워서 일하라고 알림
    _cv.notify_one(); 
}

void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(_m);
            
            // 큐에 일이 없으면 잠듬 (일이 들어오거나, 종료 신호가 올 때까지 대기)
            _cv.wait(lock, [this]() { return !this->_jobs.empty() || this->_stop; });

            // 종료 신호이고 큐도 비었으면 루프 탈출 (스레드 종료)
            if (this->_stop && this->_jobs.empty())
                return;

            // 큐에서 일감 하나 꺼냄
            job = std::move(_jobs.front());
            _jobs.pop();
        } // lock 해제 (일 처리는 락 없이 수행해야 다른 스레드도 큐에 접근 가능)

        // 실제 일감 수행
        job();
    }
}