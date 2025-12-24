
----


스레드를 효율적으로 대기하는 동기화 객체
Busy Wait을 막기 위해서 이벤트를 통해서 값을 제어

```cpp

#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex m;
std::condition_variable cv;
std::queue<int> taskQueue;

// [Consumer 스레드: 일꾼]
void Worker() {
    while (true) {
        // 1. unique_lock이 필수입니다. (wait 내부에서 락을 풀었다 잡았다 해야 함)
        std::unique_lock<std::mutex> lock(m);

        // 2. wait(락, 조건)
        // 조건이 false면 락을 풀고 잠듭니다. 
        // 깨어났을 때 조건이 true면 다시 락을 잡고 진행합니다.
        cv.wait(lock, [] { return !taskQueue.empty(); });

        // 3. 일 처리
        int task = taskQueue.front();
        taskQueue.pop();
        lock.unlock(); // 일 처리 전 락을 빨리 풀어주는 것이 서버 성능에 좋습니다.
        
        Process(task);
    }
}

// [Producer 스레드: 일 주는 쪽]
void PushTask(int data) {
    {
        std::lock_guard<std::mutex> lock(m);
        taskQueue.push(data);
    }
    // 4. 잠자고 있는 스레드 하나를 깨웁니다.
    cv.notify_one(); 
}

```


가짜 깨어남 쓰면 안됨.
