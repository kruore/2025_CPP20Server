
---


RAII(Resource Acquisition Is Initialization)

RAII는 직역하면 **"자원의 획득은 초기화다"**라는 뜻이지만, 더 쉽게 풀이하면 **"자원의 수명을 객체의 수명에 바인딩(Binding)하는 것"**

- **생성자 (Acquire):** 객체가 생성될 때 자원(메모리, 뮤텍스 락, 파일 핸들 등)을 획득합니다.
    
- **소멸자 (Release):** 객체가 스코프(Scope)를 벗어나 소멸될 때 자원을 자동으로 해제합니다.


``` cpp

void ProcessData() {
    _mutex.lock(); // 락 획득

    if (IsError()) { 
        // _mutex.unlock(); 을 깜빡하고 리턴한다면? -> 데드락 발생!
        return; 
    }

    // 예외가 발생한다면? -> 데드락 발생!
    DoSomething(); 

    _mutex.unlock(); // 락 해제
}



void ProcessData() {
    // 생성자에서 락을 걸고, 함수가 어떻게 끝나든 소멸자에서 락을 해제함
    std::lock_guard<std::mutex> lock(_mutex); 

    if (IsError()) {
        return; // 자동으로 소멸자가 호출되어 락 해제
    }

    DoSomething(); // 예외가 발생해도 스택 풀기(Stack Unwinding)로 락 해제
}

```

